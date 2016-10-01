#include "syncengine.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "utils/jsonparser.h"
#include "database/databasemanager.h"

SyncEngine::SyncEngine(QObject *parent) : QThread(parent)
{
}

void SyncEngine::run()
{
    m_networkAccessManager = new QNetworkAccessManager();
    exec();
}

void SyncEngine::setUserToken(QString token)
{
    m_userToken = token;
}

void SyncEngine::saveNotes(QJsonArray notesList)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    DatabaseManager::instance()->database().transaction();
    query.prepare("INSERT INTO nota (resource_id, texto, data_hora) VALUES (?1, ?2, ?3)");
    QJsonArray::const_iterator iterator;
    QString id;
    QString text;
    qlonglong dateTime;
    for(iterator = notesList.constBegin(); iterator != notesList.constEnd(); ++iterator) {
        id = (*iterator).toObject().value("_id").toString();
        text = (*iterator).toObject().value("text").toString();
        dateTime = (*iterator).toObject().value("date").toVariant().toDateTime().toTime_t();
        query.bindValue(0,id);
        query.bindValue(1,text);
        query.bindValue(2,dateTime);
        query.exec();
    }
    DatabaseManager::instance()->database().commit();

    emit downloadNotesFinished();

}

void SyncEngine::downloadNotes()
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "downloadNotes", Qt::QueuedConnection);
        return;
    }
    if (m_userToken.isEmpty()) {
        emit downloadError("Token inválido");
        return;
    }

    QNetworkRequest request = prepareRequest(API_ADRESS"/notes");
    QNetworkReply *reply = m_networkAccessManager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        QString content = reply->readAll();
        if (statusCode != 200) {
            QString errorMsg = "";
            if (statusCode == 401) {
                errorMsg = "Token invalido";
            } else {
                errorMsg = reply->errorString();
            }
            emit downloadError(errorMsg);
        } else {
            saveNotes(JSONParser::parseToArray(content));
        }
        reply->deleteLater();
    }, Qt::QueuedConnection);
}

QNetworkRequest SyncEngine::prepareRequest(QString urlString)
{
    QUrl url(urlString);
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request.setRawHeader("x-access-token",m_userToken.toLatin1());
    return request;
}

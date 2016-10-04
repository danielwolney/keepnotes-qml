#include "syncengine.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "utils/jsonparser.h"
#include "database/databasemanager.h"
#include "control/usermanager.h"
#include "model/user.h"

SyncEngine::SyncEngine(QObject *parent) : QThread(parent),
    m_user(NULL)
{
    m_timerSync = new QTimer(this);
    m_timerSync->setInterval(5*1000);
}

void SyncEngine::run()
{
    connect(m_timerSync, &QTimer::timeout, this, &SyncEngine::triggerSync);
    m_networkAccessManager = new QNetworkAccessManager();
    if (m_userManager->user() != NULL) {
        m_user = m_userManager->user();
        m_timerSync->start();
    }
    exec();
}

void SyncEngine::setUserManager(UserManager *manager)
{
    m_userManager = manager;
    connect(m_userManager, &UserManager::login, this, &SyncEngine::downloadNotes);
    connect(m_userManager, &UserManager::logout, this, &SyncEngine::cancelSync);
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
    m_timerSync->start();
}

#include <QDebug>
void SyncEngine::triggerSync()
{
    qDebug() << "SYNC";
}

void SyncEngine::cancelSync()
{
    m_timerSync->stop();
    qDebug() << "CANCEL SYNC";
}

void SyncEngine::downloadNotes()
{
    if (QThread::currentThread() != thread()) {
        QMetaObject::invokeMethod(this, "downloadNotes", Qt::QueuedConnection);
        return;
    }
    m_user = m_userManager->user();

    if (m_user->token().isEmpty()) {
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
    request.setRawHeader("x-access-token",m_user->token().toLatin1());
    return request;
}

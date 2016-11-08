#include "syncengine.h"
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "utils/jsonparser.h"
#include "database/databasemanager.h"
#include "control/usermanager.h"
#include "model/user.h"
#include <QBuffer>

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

void SyncEngine::triggerSync()
{
    m_timerSync->stop();
    sendInserts();
}

void SyncEngine::cancelSync()
{
    m_timerSync->stop();
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

void SyncEngine::sendInserts()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT id, texto, data_hora FROM nota WHERE resource_id IS NULL OR resource_id = ''");
    query.exec();
    if (query.first()) {
        QJsonArray noteArray;
        do {
            QJsonObject noteObj;
            noteObj.insert("localID", QJsonValue(query.value("id").toInt()));
            noteObj.insert("text", QJsonValue(query.value("texto").toString()));
            noteObj.insert("date", QJsonValue(query.value("data_hora").toLongLong()));
            noteArray.append(QJsonValue(noteObj));
        } while(query.next());
        QJsonDocument document;
        document.setArray(noteArray);
        QNetworkRequest request = prepareRequest(API_ADRESS"/notes");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = m_networkAccessManager->post(request,document.toJson());
        connect(reply, &QNetworkReply::finished, this, [=]() {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QJsonObject resObj = JSONParser::parseToObject(reply->readAll());
                QJsonArray savedNotes = resObj.value("savedNotes").toArray();
                confirmInserts(savedNotes);
            } else {
                m_timerSync->start();
            }
            reply->deleteLater();
        }, Qt::QueuedConnection);
    } else {
        sendDeletes();
    }
}

void SyncEngine::confirmInserts(QJsonArray savedNotes)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("UPDATE nota SET resource_id = ?1 WHERE id = ?2");
    QJsonArray::ConstIterator it = savedNotes.constBegin();
    while (it != savedNotes.constEnd()) {
        query.bindValue(0, (*it).toObject().value("resourceID").toString());
        query.bindValue(1, (*it).toObject().value("localID").toInt());
        query.exec();
        ++it;
    }
    sendDeletes();
}

void SyncEngine::sendDeletes()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT resource_id FROM info_sync_nota WHERE tipo_sync = 'delete'");
    query.exec();
    if (query.first()) {
        QJsonArray noteArray;
        do {
            noteArray.append(QJsonValue(query.value("resource_id").toString()));
        } while(query.next());
        QJsonDocument document;
        document.setArray(noteArray);
        QNetworkRequest request = prepareRequest(API_ADRESS"/notes");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QByteArray *json = new QByteArray(document.toJson());
        QBuffer *buffer = new QBuffer(json);
        QNetworkReply *reply = m_networkAccessManager->sendCustomRequest(request,"DELETE", buffer);
        connect(reply, &QNetworkReply::finished, this, [=]() {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QJsonObject resObj = JSONParser::parseToObject(reply->readAll());
                QJsonArray deletedNotes = resObj.value("deletedNotes").toArray();
                confirmDeletes(deletedNotes);
            } else {
                m_timerSync->start();
            }
            reply->deleteLater();
            buffer->deleteLater();
            delete json;
        }, Qt::QueuedConnection);
    } else {
        sendUpdates();
    }
}

void SyncEngine::confirmDeletes(QJsonArray deletedNotes)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("DELETE FROM info_sync_nota WHERE resource_id = ?1");
    QJsonArray::ConstIterator it = deletedNotes.constBegin();
    while (it != deletedNotes.constEnd()) {
        query.bindValue(0, (*it).toString());
        query.exec();
        ++it;
    }
    sendUpdates();
}

void SyncEngine::sendUpdates()
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("SELECT nota.resource_id, texto, data_hora "
                  "FROM nota "
                  "JOIN info_sync_nota info "
                  "ON info.resource_id = nota.resource_id "
                  "WHERE info.tipo_sync = 'update'; ");
    query.exec();
    if (query.first()) {
        QJsonArray noteArray;
        do {
            QJsonObject noteObj;
            noteObj.insert("resourceID", QJsonValue(query.value("resource_id").toString()));
            noteObj.insert("text", QJsonValue(query.value("texto").toString()));
            noteObj.insert("date", QJsonValue(query.value("data_hora").toLongLong()));
            noteArray.append(QJsonValue(noteObj));
        } while(query.next());
        QJsonDocument document;
        document.setArray(noteArray);
        QNetworkRequest request = prepareRequest(API_ADRESS"/notes");
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QNetworkReply *reply = m_networkAccessManager->put(request,document.toJson());
        connect(reply, &QNetworkReply::finished, this, [=]() {
            int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 200) {
                QJsonObject resObj = JSONParser::parseToObject(reply->readAll());
                QJsonArray updatedNotes = resObj.value("updatedNotes").toArray();
                confirmUpdates(updatedNotes);
            } else {
                qDebug() << "statusCode" << statusCode << reply->errorString();
                m_timerSync->start();
            }
            reply->deleteLater();
        }, Qt::QueuedConnection);
    } else {
        m_timerSync->start();
    }
}

void SyncEngine::confirmUpdates(QJsonArray updatedNotes)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    query.prepare("DELETE FROM info_sync_nota WHERE resource_id = ?1 AND tipo_sync = 'update'");
    QJsonArray::ConstIterator it = updatedNotes.constBegin();
    while (it != updatedNotes.constEnd()) {
        query.bindValue(0, (*it).toString());
        query.exec();
        ++it;
    }
    m_timerSync->start();
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

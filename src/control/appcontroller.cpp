#include "appcontroller.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "sync/syncengine.h"
#include "utils/jsonparser.h"
#include "usermanager.h"
#include "model/user.h"
#include "model/notesmodel.h"

AppController::AppController(QObject *parent) : QObject(parent)
  , m_engine(new QQmlApplicationEngine())
  , m_userManager(new UserManager)
  , m_syncEngine(new SyncEngine())
{
    m_engine->rootContext()->setContextProperty("app", this);
    m_syncEngine->start();
    m_syncEngine->moveToThread(m_syncEngine);
}

NotesModel *AppController::notes()
{
    return m_notesModel;
}

void AppController::startApplication()
{
    if (m_userManager->user() != NULL) {
        m_syncEngine->setUserToken(m_userManager->user()->token());
        showMainWindow();
    } else {
        showLoginWindow();
    }
}

void AppController::showLoginWindow()
{
    m_engine->load(QUrl(QStringLiteral("qrc:/views/login.qml")));
}

void AppController::showMainWindow()
{
    m_notesModel = new NotesModel(this);
    m_engine->load(QUrl(QStringLiteral("qrc:/views/main.qml")));
}

void AppController::saveLoginInfo(QString email, QString jsonLogin)
{
    QJsonObject responseObject = JSONParser::parseToObject(jsonLogin);
    QString token = responseObject.value("token").toString();
    m_userManager->setUser(email, token);
    m_syncEngine->setUserToken(token);
    downloadInitialData();
}

void AppController::downloadInitialData()
{
    connect(m_syncEngine, &SyncEngine::downloadNotesFinished, this, &AppController::completeLogin);
    m_syncEngine->downloadNotes();
}

void AppController::completeLogin()
{
    disconnect(m_syncEngine, &SyncEngine::downloadNotesFinished, this, &AppController::completeLogin);
    showMainWindow();
    emit loginSuccessful();
}

void AppController::login(QString email, QString password)
{
    QUrl url(API_ADRESS"/login");
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QString data = "email="+email+"&password="+password;
    QNetworkAccessManager * networkManager = new QNetworkAccessManager();
    QNetworkReply *reply = networkManager->post(request,data.toLatin1());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (statusCode != 200) {
            QString errorMsg = "";
            if (statusCode == 401) {
                errorMsg = "Usuário ou senha inválida.";
            } else {
                errorMsg = reply->errorString();
            }
            emit loginError(errorMsg);
        } else {
            QString content = reply->readAll();
            saveLoginInfo(email, content);
        }
        reply->deleteLater();
        networkManager->deleteLater();
    }, Qt::QueuedConnection);
}


#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class QNetworkReply;
class QQmlApplicationEngine;
class UserManager;
class SyncEngine;

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = 0);
signals:
    void loginSuccessful();
    void loginError(QString errorMessage);

public slots:
    void startApplication();
    void login(QString email, QString password);

private:
    QQmlApplicationEngine* m_engine;
    UserManager *m_userManager;
    SyncEngine *m_syncEngine;

    void showLoginWindow();
    void showMainWindow();
    void saveLoginInfo(QString email, QString jsonLogin);
    Q_INVOKABLE void completeLogin();


    void downloadInitialData();
};

#endif // APPCONTROLLER_H

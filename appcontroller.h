#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <QObject>

class QNetworkReply;
class QQmlApplicationEngine;
class UserManager;
class SyncEngine;

class NotesModel;

class AppController : public QObject
{
    Q_OBJECT
public:
    explicit AppController(QObject *parent = 0);
    Q_PROPERTY(NotesModel* notes READ notes CONSTANT)
    NotesModel *notes();

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
    NotesModel *m_notesModel;

    void showLoginWindow();
    void showMainWindow();
    void saveLoginInfo(QString email, QString jsonLogin);
    void downloadInitialData();
    Q_INVOKABLE void completeLogin();
};

#endif // APPCONTROLLER_H

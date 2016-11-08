#ifndef SYNCENGINE_H
#define SYNCENGINE_H

#include <QThread>
#include <QNetworkRequest>
#include <QJsonArray>
#include <QTimer>

#define API_ADRESS "https://keepnotes-api.herokuapp.com/api/v1/"

class QNetworkAccessManager;
class UserManager;
class User;

class SyncEngine : public QThread
{
    Q_OBJECT
public:
    explicit SyncEngine(QObject *parent = 0);
protected:
    void run();
signals:
    void downloadNotesFinished();
    void downloadError(QString msg);

public slots:
    void setUserManager(UserManager *manager);
    void downloadNotes();

    void sendInserts();
    void confirmInserts(QJsonArray savedNotes);

    void sendDeletes();
    void confirmDeletes(QJsonArray deletedNotes);

    void sendUpdates();
    void confirmUpdates(QJsonArray updatedNotes);

private slots:
    void saveNotes(QJsonArray notesArray);

    void triggerSync();
    void cancelSync();

private:
    UserManager *m_userManager;
    User *m_user;

    QTimer *m_timerSync;
    QNetworkAccessManager * m_networkAccessManager;
    QNetworkRequest prepareRequest(QString urlString);
};

#endif // SYNCENGINE_H

#ifndef SYNCENGINE_H
#define SYNCENGINE_H

#include <QThread>
#include <QNetworkRequest>
#include <QJsonArray>

#define API_ADRESS "https://keepnotes-api.herokuapp.com/api/v1/"

class QNetworkAccessManager;

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
    void setUserToken(QString token);
    void downloadNotes();
private slots:
    void saveNotes(QJsonArray notesArray);
private:
    QString m_userToken;

    QNetworkAccessManager * m_networkAccessManager;
    QNetworkRequest prepareRequest(QString urlString);
};

#endif // SYNCENGINE_H

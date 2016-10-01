#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QSqlDatabase>
#include <QHash>
#include <QDir>
#include <QSqlQuery>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:

    static DatabaseManager * instance();

    QSqlDatabase &database();

    ~DatabaseManager();

    QDir defaultDataDir();
signals:
    void connectionRefreshed();

public slots:
    void reconnectDB();
    void releaseConnection();

private:
    static const QString DB_VERSION_ATTRIBUTE;
    static const int DB_VERSION;
    static const QString CONNECTION_NAME;

    static DatabaseManager * s_instance;

    QSqlDatabase m_database;

    explicit DatabaseManager(QObject *parent = 0);
    QSqlDatabase openDatabase(QString databaseName, int version);
    bool createDB(QString databaseName, QSqlDatabase database);
    void upgradeDB(QString databaseName, int currentVersion, QSqlDatabase db);
    bool runSqlScript(QSqlDatabase db, QString filePath);

};

#endif // DATABASEMANAGER_H

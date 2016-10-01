#include "databasemanager.h"
#include <QSqlError>
#include <QTextStream>
#include <QSqlRecord>
#include <QHash>
#include <QStandardPaths>

const QString DatabaseManager::CONNECTION_NAME = "data";
const QString DatabaseManager::DB_VERSION_ATTRIBUTE = "db_version";
const int DatabaseManager::DB_VERSION = 1;

DatabaseManager* DatabaseManager::s_instance = NULL;

//Separador de fim de query (END OF QUERY). Irá separar as queries que estarão nos scripts
const QString QUERY_SEPARATOR = "#EOQ#";

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    reconnectDB();
}

DatabaseManager::~DatabaseManager()
{
}

QDir DatabaseManager::defaultDataDir()
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    dir.mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    return dir;
}


void DatabaseManager::reconnectDB()
{
    m_database = openDatabase(CONNECTION_NAME, DB_VERSION);
    emit connectionRefreshed();
}

void DatabaseManager::releaseConnection()
{
    m_database.close();
}

DatabaseManager *DatabaseManager::instance()
{
    if (s_instance == NULL) {
        s_instance = new DatabaseManager();
    }
    return s_instance;
}

QSqlDatabase &DatabaseManager::database()
{
    return m_database;
}

bool DatabaseManager::createDB(QString databaseName, QSqlDatabase database)
{
    bool ok = runSqlScript(database, ":/sql/create_"+databaseName+".sql");
    return ok;
}

QSqlDatabase DatabaseManager::openDatabase(QString databaseName, int version)
{
    if (!QSqlDatabase::contains(databaseName)){
        QSqlDatabase::addDatabase("QSQLITE",databaseName);
    }
    QSqlDatabase db = QSqlDatabase::database(databaseName);
    QDir dataDir = defaultDataDir();
    db.setDatabaseName(dataDir.absolutePath() + "/" + databaseName + ".db3");
    bool fileExists = dataDir.exists(db.databaseName());
    db.open();
    if(!fileExists) {
        createDB(databaseName, db);
    } else {
        QSqlQuery query(db);
        if (query.prepare("SELECT value FROM properties WHERE key = ?1")) {
            query.bindValue(0, version);
            if (query.exec() && query.first()) {
                upgradeDB(databaseName, query.record().value(0).toInt(), db);
            } else {
                upgradeDB(databaseName, 1, db);
            }
        }
    }


   return db;
}

void DatabaseManager::upgradeDB(QString databaseName, int currentVersion, QSqlDatabase db)
{
    switch (currentVersion) {
    case 1:
        runSqlScript(db, ":/sql/upgrade_"+databaseName+"_v2.sql");
/**
 * Adicionar mais clásulas cases sem o break para cada versão do banco que quiser atualizar.
 * A inexistencia do break ira garantir que os patches de cada versao ira rodar.
 *
 * e.g.:
 *  case 2:
 *     runSqlScript(db, ":/upgrade_<NOME_BANCO>_v3"");
 *  case 3:
 *     runSqlScript(db, ":/upgrade_<NOME_BANCO>_v4");
 */
        break;
    default:
        break;
    }

}

bool DatabaseManager::runSqlScript(QSqlDatabase database, QString filePath)
{
    bool allQueriesSuccessful = false;
    if (database.isValid() && database.isOpen()) {
        allQueriesSuccessful = true;
        QFile scriptSqlFile(filePath);
        if (scriptSqlFile.open(QIODevice::ReadOnly)) {
            QStringList scriptQueries = QTextStream(&scriptSqlFile).readAll().split(QUERY_SEPARATOR);
            database.transaction();
            foreach (QString queryTxt, scriptQueries) {
                if (queryTxt.trimmed().isEmpty()) {
                    continue;
                }
                if (queryTxt.contains("DROP TABLE")) {
                    database.commit();
                    database.close();
                    database.open();
                    database.transaction();
                }
                if (database.exec(queryTxt.trimmed()).lastError().isValid()) {
                    QString errorMessage = "One of the query failed to execute."
                                           "\n"  + queryTxt +
                                           "\nError detail: " + database.lastError().text();
                    database.rollback();
                    database.close();
                    QFile::remove(database.databaseName());
                    qFatal(errorMessage.toLatin1(),"") ;
                } else {
                    allQueriesSuccessful &= true;
                }
            }
        }
        database.commit();
    }
    return allQueriesSuccessful;
}



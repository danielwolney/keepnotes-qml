#include "usermanager.h"
#include "model/user.h"
#include "database/databasemanager.h"
#include <QSqlQuery>
#include <QVariant>

UserManager::UserManager(QObject *parent) : QObject(parent)
  , m_user(NULL)
{
    QSqlQuery query(DatabaseManager::instance()->database());
    if (query.exec("SELECT email, token FROM usuario") && query.first()) {
        m_user = new User();
        m_user->setEmail(query.value(0).toString());
        m_user->setToken(query.value(1).toString());
    }
}

User *UserManager::user() const
{
    return m_user;
}

void UserManager::setUser(const QString &email, const QString &token)
{
    m_user = new User();
    m_user->setEmail(email);
    m_user->setToken(token);
    QSqlQuery query(DatabaseManager::instance()->database());
    query.exec("DELETE FROM usuario");
    query.prepare("INSERT INTO usuario (email, token) VALUES (?1, ?2)");
    query.bindValue(0, email);
    query.bindValue(1, token);
    query.exec();
    emit login();
}

void UserManager::invalidateUser()
{
    if (m_user) {
        emit logout();
        QSqlQuery query(DatabaseManager::instance()->database());
        query.exec("DELETE FROM usuario");
        m_user->deleteLater();
        m_user = NULL;
        DatabaseManager::instance()->releaseConnection();
        DatabaseManager::instance()->reconnectDB();
    }
}

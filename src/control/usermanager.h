#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>

class User;

class UserManager : public QObject
{
    Q_OBJECT
public:
    explicit UserManager(QObject *parent = 0);

    User *user() const;
    void setUser(const QString &email, const QString &token);
signals:
    void login();
    void logout();

public slots:
    void invalidateUser();

private:
    User *m_user;
};

#endif // USERMANAGER_H

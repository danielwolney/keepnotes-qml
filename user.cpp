#include "user.h"

User::User(QObject *parent) : QObject(parent)
{
}

QString User::email() const
{
    return m_email;
}

QString User::token() const
{
    return m_token;
}

void User::setEmail(QString email)
{
    if (m_email == email)
        return;

    m_email = email;
    emit emailChanged(email);
}

void User::setToken(QString token)
{
    if (m_token == token)
        return;

    m_token = token;
    emit tokenChanged(token);
}

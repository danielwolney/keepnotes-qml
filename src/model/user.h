#ifndef USER_H
#define USER_H

#include <QObject>

class User : public QObject
{
    Q_OBJECT
public:
    explicit User(QObject *parent = 0);
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged)

    QString email() const;
    QString token() const;

signals:
    void emailChanged(QString email);
    void tokenChanged(QString token);

public slots:
    void setEmail(QString email);
    void setToken(QString token);

private:
    QString m_email;
    QString m_token;
};

#endif // USER_H

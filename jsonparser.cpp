#include "jsonparser.h"

QJsonObject JSONParser::parseToObject(QString jsonContent)
{
    QJsonDocument rootDocument(QJsonDocument::fromJson(jsonContent.toLatin1()));
    QJsonObject object = rootDocument.object();
    return object;
}

QJsonArray JSONParser::parseToArray(QString jsonContent)
{
    QJsonDocument rootDocument(QJsonDocument::fromJson(jsonContent.toLatin1()));
    QJsonArray array = rootDocument.array();
    return array;
}

#ifndef JSONPARSER_H
#define JSONPARSER_H
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

namespace JSONParser {
QJsonObject parseToObject(QString jsonContent);
QJsonArray parseToArray(QString jsonContent);
}

#endif // JSONPARSER_H

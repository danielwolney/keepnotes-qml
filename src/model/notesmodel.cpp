#include "notesmodel.h"
#include <QSqlTableModel>
#include "database/databasemanager.h"
#include <QThread>

NotesModel::NotesModel(QObject *parent) :
    QAbstractListModel(parent),
    m_tableModel(new QSqlTableModel(this, DatabaseManager::instance()->database()))
{
    connect(DatabaseManager::instance(), &DatabaseManager::connectionRefreshed, this, &NotesModel::refreshTable);

    registerRoleColum(ID, "id");
    registerRoleColum(ResourceID, "resource_id");
    registerRoleColum(Text, "texto");
    registerRoleColum(Datetime, "data_hora");

    m_tableName = "nota";
    m_tableModel->setTable(m_tableName);
    m_tableModel->setSort(columTableIndex("data_hora"),Qt::DescendingOrder);
    m_tableModel->select();
}

void NotesModel::fetchMore(const QModelIndex &parent)
{
    Q_UNUSED(parent)
    int rowCountBeforeFetch = m_tableModel->rowCount();
    m_tableModel->fetchMore();
    int remainder = m_tableModel->rowCount() - rowCountBeforeFetch;
    int itemsToFetch = qMin(256, remainder);
    beginInsertRows(QModelIndex(), rowCountBeforeFetch, rowCountBeforeFetch+itemsToFetch-1);
    endInsertRows();
}

bool NotesModel::canFetchMore(const QModelIndex &parent) const
{
    return m_tableModel->canFetchMore(parent);
}

QHash<int, QByteArray> NotesModel::roleNames() const
{
    QHash<int, QByteArray> names;
    names[ID] = "id";
    names[ResourceID] = "resourceID";
    names[Text] = "text";
    names[Datetime] = "datetime";
    return names;
}

void NotesModel::addNote(QString text)
{
    if (!text.isEmpty()) {
        beginInsertRows(QModelIndex(), 0/*rowCount()*/,0 /*rowCount()*/);
        int newRow = 0;//m_tableModel->rowCount();
        m_tableModel->insertRow(newRow);
        int columnText = columTableIndex("texto");
        m_tableModel->setData(m_tableModel->index(newRow, columnText),text);
        m_tableModel->submitAll();
        endInsertRows();
    }
}

void NotesModel::updateNote(int row, QString text)
{

    QModelIndex textIndex = m_tableModel->index(row, columTableIndex("texto"));
    if (m_tableModel->setData(textIndex, text) && m_tableModel->submitAll()) {
        emit dataChanged(index(row), index(row));
    }
}

bool NotesModel::removeNote(int row)
{
    return removeRow(row);
}

bool NotesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    bool success = m_tableModel->removeRows(row, count, parent);
    if (success) {
        m_tableModel->submitAll();
        beginRemoveRows(parent,row,row+count-1);
        m_tableModel->setSort(columTableIndex("data_hora"),Qt::DescendingOrder);
        m_tableModel->select();
        endRemoveRows();
    } else {
        m_tableModel->revert();
    }
    return success;
}

void NotesModel::refreshTable()
{
    if (QThread::currentThread() != thread()) {
        //Chamar via invoke para o método ser executado na mesma Thread onde o objeto foi criado
        QMetaObject::invokeMethod(this, "refreshTable");
        return;
    }

    beginResetModel();
    if (m_tableModel) {
        delete m_tableModel;
    }
    m_tableModel = new QSqlTableModel(this, DatabaseManager::instance()->database());
    m_tableModel->setTable(m_tableName);
    m_tableModel->setSort(columTableIndex("data_hora"),Qt::DescendingOrder);
    m_tableModel->select();
    endResetModel();
}

QString NotesModel::text(int row)
{
    return data(row, Text).toString();
}

QDateTime NotesModel::dateTime(int row)
{
    return QDateTime::fromTime_t(data(row, Datetime).toLongLong());
}

int NotesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_tableModel->rowCount();
}

QVariant NotesModel::data(const QModelIndex &index, int role) const
{
    return mappedData(index, role);
}

QVariant NotesModel::data(int row, int role) const
{
    return data(index(row), role);
}

void NotesModel::registerRoleColum(int role, QString columnName)
{
    m_roleColumn.insert(role, columnName);
}

QVariant NotesModel::mappedData(const QModelIndex &index, int role) const
{
    if (m_roleColumn.contains(role)) {
        int column = columTableIndex(m_roleColumn.value(role) );
        QModelIndex itemListIndex = m_tableModel->index(index.row(), column);
        return m_tableModel->data(itemListIndex);
    }
    return QVariant();
}

int NotesModel::columTableIndex(QString columnName) const
{
    return m_tableModel->fieldIndex(columnName);
}

#include <QtQml>
void qmlRegisterNotesModel()
{
    qmlRegisterType<NotesModel>("Models", 1, 0, "NotesModel");
}
Q_COREAPP_STARTUP_FUNCTION(qmlRegisterNotesModel)

#include "notesmodel.h"
#include <QSqlTableModel>
#include "databasemanager.h"
#include <QThread>

NotesModel::NotesModel(QObject *parent) :
    QAbstractListModel(parent),
    m_tableModel(new QSqlTableModel(this, DatabaseManager::instance()->database()))
{
    connect(DatabaseManager::instance(), &DatabaseManager::connectionRefreshed, this, &NotesModel::refreshTable);
    m_tableName = "nota";
    m_tableModel->setTable(m_tableName);
    m_tableModel->select();
    registerRoleColum(ID, "id");
    registerRoleColum(ResourceID, "resource_id");
    registerRoleColum(Text, "texto");
    registerRoleColum(Datetime, "data_hora");
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
    m_tableModel->select();
    endResetModel();
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

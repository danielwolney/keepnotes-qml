#ifndef NOTESMODEL_H
#define NOTESMODEL_H

#include <QAbstractListModel>
#include <QDateTime>

class QSqlTableModel;

class NotesModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Column {
        ID = Qt::UserRole + 1,
        ResourceID,
        Text,
        Datetime
    };

    explicit NotesModel(QObject *parent = 0);
    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
    Q_INVOKABLE void fetchMore(const QModelIndex &parent) Q_DECL_OVERRIDE;
    Q_INVOKABLE bool canFetchMore(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    Q_INVOKABLE QHash<int,QByteArray> roleNames() const Q_DECL_OVERRIDE;
    virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

public slots:
    void addNote(QString text);
    void updateNote(int row, QString text);
    bool removeNote(int row);
    QString text(int row);
    QDateTime dateTime(int row);
    void refreshTable();
private:
    void registerRoleColum(int role, QString columnName);
    QVariant mappedData(const QModelIndex &index, int role) const;
    int columTableIndex(QString columnName) const;
    QVariant data(int row, int role) const;
    QSqlTableModel *m_tableModel;
    QString m_tableName;
    QHash<int,QString> m_roleColumn;
};

#endif // NOTESMODEL_H

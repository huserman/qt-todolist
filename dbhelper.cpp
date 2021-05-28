#include <QSqlQuery>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QtDebug>

#include "dbhelper.h"

DbHelper::DbHelper()
{
}

void DbHelper::ConnectToDb()
{

    if (QSqlDatabase::contains(connectionName))
    {
        db = QSqlDatabase::database(connectionName);
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    db.setDatabaseName(connectionString);

    if (!db.open())
        qDebug() << "Error: connection with database failed";
    else
        qDebug() << "Database: connection opened";
}

DbHelper::~DbHelper()
{
    if (db.isOpen())
    {
        db.close();
        qDebug() << "Database: connection closed";
        removeDb(connectionName);
    }
}

bool DbHelper::isOpen() const
{
    return db.isOpen();
}

void DbHelper::removeDb(const QString &conName)
{
    QSqlDatabase::removeDatabase(conName);
    qDebug() << "Database: connection removed";
}



QSqlQueryModel* DbHelper::getRecords() const
{
    QSqlQueryModel* model = new QSqlQueryModel;
    QString q = "SELECT id, done, description FROM mylist WHERE isdeleted = 0";
    model->setQuery(q);
    return model;
}

bool DbHelper::updateRecords(const TodoItem &item)
{
    bool success = false;

    if (!item.description.isEmpty())
    {
        QSqlQuery query(db);
        QString strQuery = "UPDATE mylist SET done = :done, description = :description, addedtime = datetime(CURRENT_TIMESTAMP, 'localtime') "
                           "WHERE id = :id";
        query.prepare(strQuery);
        query.bindValue(":done", QString::number(item.done));
        query.bindValue(":description", item.description);
        query.bindValue(":id", item.id);

        if (!query.exec())
        {
            qDebug() << query.lastError();
        }
        else
        {
            success = true;
            qDebug() << "Updated: id#" << item.id;
        }
    }
    else
    {
        qDebug() << "Update failed: description cannot be empty";
    }
    return success;
}

bool DbHelper::insertRecords(const TodoItem& item)
{
    bool success = false;

    if (!item.description.isEmpty())
    {
        QSqlQuery queryAdd;
        queryAdd.prepare("INSERT INTO mylist(done, description, addedtime) "
                         "VALUES (:done, :description, datetime(CURRENT_TIMESTAMP, 'localtime'))");
        queryAdd.bindValue(":done", item.done);
        queryAdd.bindValue(":description", item.description);

        if (queryAdd.exec())
        {
            success = true;
            qDebug() << "Inserted!";
        }
        else
        {
            qDebug() << "Insertion failed: " << queryAdd.lastError();
        }
    }
    else
    {
        qDebug() << "Insertion failed: description cannot be empty";
    }

    return success;
}

bool DbHelper::deleteRecordByID(const quint32 itemId)
{
    bool success = false;

    QSqlQuery query(db);
    QString strQuery = "UPDATE mylist SET isdeleted = 1, addedtime = datetime(CURRENT_TIMESTAMP, 'localtime') "
                       "WHERE id = :id";
    query.prepare(strQuery);
    query.bindValue(":id", itemId);

    if (!query.exec())
    {
        qDebug() << query.lastError();
    }
    else
    {
        success = true;
        qDebug() << "Deleted: id#" << itemId;
    }

    return success;
}

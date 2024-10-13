#include "DbCtrl.h"

DbCtrl *DbCtrl::m_dbCtrl = nullptr;

DbCtrl::DbCtrl(QString dbName)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        m_database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        m_database = QSqlDatabase::addDatabase("QSQLITE");
        m_database.setHostName("127.0.0");
        m_database.setDatabaseName(dbName);
        m_database.setUserName("user");
        m_database.setPassword("pwssword");
    }

    if(!m_database.open())
    {
        qCritical()<<"Fail to create or connect database. "<<m_database.lastError();
    }
    else
    {
        qDebug()<<"Success to connect local database. "<<dbName;
    }
}

bool DbCtrl::RunSql(QString strSql)
{
    QSqlQuery query;

    if(query.exec(strSql) == false)
    {
        qCritical()<<"Fail to run sql: "<<strSql<<" | "<<m_dbCtrl->m_database.lastError();
        return false;
    }
    else
    {
        qDebug()<<"run sql successfully. "<<strSql;
        return true;
    }
}
bool DbCtrl::Init(QString dbName)
{
    if(m_dbCtrl == NULL)
    {
        m_dbCtrl = new DbCtrl(dbName);
    }

    if(m_dbCtrl->m_database.isOpen())
        return true;
    else
        return false;
}

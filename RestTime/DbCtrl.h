#ifndef DBCTRL_H
#define DBCTRL_H

#include <QString>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

typedef struct InfoNode
{
    QString     m_strName;      //名字
    QString     m_strSex;       //性别
    float       m_fWeight;      //体重
    QString     m_strHoliday;   //生理期日期
    int         m_nDays;        //持续天数
}INFONODE;

class DbCtrl
{
private:
    DbCtrl(QString dbName);

    static DbCtrl *m_dbCtrl;
    QSqlDatabase m_database;

public:
    //增删查改
    static bool RunSql(QString strSql);
    static bool Init(QString dbName = QString("localdb.db"));
};

#endif // DBCTRL_H

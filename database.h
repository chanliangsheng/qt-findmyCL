#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <string>
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <databaserecord.h>

class Database
{
public:
    Database();
public:
    QSqlDatabase m_database;//数据库
public:
    //数据库
    std::vector<DatabaseRecord> m_CL_M_H;
    std::vector<DatabaseRecord> m_CL_M_2H;
    std::vector<DatabaseRecord> m_MLCL_M_H;
    std::vector<DatabaseRecord> m_MLCL_M_2H;
    std::vector<DatabaseRecord> m_DLCL_M_H;
    std::vector<DatabaseRecord> m_DLCL_M_2H;
    std::vector<DatabaseRecord> m_PA;
    std::vector<DatabaseRecord> m_FA;
public:
    void LoadDatebase(QString file_name = "db/database.db");//加载数据库
    void LoadAllTable();//加载数据库中的所有数据表
    std::vector<DatabaseRecord>* LoadSingelTable(QString table);//加载数据库中的单个数据表
};







#endif // DATABASE_H

#include "database.h"

Database::Database(){

}


void Database::LoadDatebase(QString file_name)
{
    QSqlDatabase database;
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName(file_name);
    }


    if (!database.open())
    {
        qDebug() << "Error: Failed to connect database." << database.lastError();
    }
    else
    {
        this->m_database = database;
    }
}


void Database::LoadAllTable()
{
    //加载数据库
    this->LoadDatebase();

    //加载所有数据库，并交换数据
    this->m_CL_M_H.swap(*(this->LoadSingelTable("cl_m_h")));
    this->m_CL_M_2H.swap(*(this->LoadSingelTable("cl_m_2h")));
    this->m_MLCL_M_H.swap(*(this->LoadSingelTable("mlcl_m_h")));
    this->m_MLCL_M_2H.swap(*(this->LoadSingelTable("mlcl_m_2h")));
    this->m_DLCL_M_H.swap(*(this->LoadSingelTable("dlcl_m_h")));
    this->m_DLCL_M_2H.swap(*(this->LoadSingelTable("dlcl_m_2h")));
    this->m_PA.swap(*(this->LoadSingelTable("pa")));
    this->m_FA.swap(*(this->LoadSingelTable("fa")));

    //关闭数据库
    this->m_database.close();
}


std::vector<DatabaseRecord> *Database::LoadSingelTable(QString table)
{
    //结果初始化
    std::vector<DatabaseRecord>* ret = new std::vector<DatabaseRecord>;

    //绑定某个数据库
    QSqlQuery sql_query(this->m_database);
    QString select_all_sql = "select * from " + table + ";";
    sql_query.prepare(select_all_sql);

    if(!sql_query.exec())
    {
      qDebug()<<sql_query.lastError();
    }
    else
    {
      while(sql_query.next())
      {
        ret->emplace_back(DatabaseRecord(sql_query.value(0).toFloat() ,
                                         sql_query.value(1).toString().toStdString() ,
                                         sql_query.value(4).toString().toStdString(),
                                         sql_query.value(3).toUInt(),
                                         sql_query.value(2).toUInt(),
                                         sql_query.value(5).toUInt()));
      }
    }

    return ret;
}

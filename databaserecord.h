#ifndef DATABASERECORD_H
#define DATABASERECORD_H

#include <vector>
#include <string>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseRecord
{
public:
    DatabaseRecord();
    DatabaseRecord(float mz , std::string additive_form , std::string formula ,uint chain_length , uint unsaturation , uint oxygen);
public:
    float m_mz;
    std::string m_additive_form;
    std::string m_formula;
    unsigned int m_chain_length;
    unsigned int m_unsaturation;
    unsigned int m_oxygen;
};

#endif // DATABASERECORD_H

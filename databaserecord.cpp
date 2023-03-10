#include "databaserecord.h"

DatabaseRecord::DatabaseRecord()
{

}

DatabaseRecord::DatabaseRecord(float mz, std::string additive_form, std::string formula , uint chain_length, uint unsaturation, uint oxygen)
{
    //有参构造函数
    this->m_mz = mz;
    this->m_additive_form = additive_form;
    this->m_formula = formula;
    this->m_chain_length = chain_length;
    this->m_unsaturation = unsaturation;
    this->m_oxygen = oxygen;
}

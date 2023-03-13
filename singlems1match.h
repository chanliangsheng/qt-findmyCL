#ifndef SINGLEMS1MATCH_H
#define SINGLEMS1MATCH_H

#include <database.h>
#include <mzml.h>
#include <memory>

//一个一级只和一个配对结果进行存储，因为要用于M-H和M-2H的配对，配对是利用配对结果进行计算的，一对一地配对
class SingleMs1Match
{
public:
    SingleMs1Match();//默认构造函数
    SingleMs1Match(Ms1* Ms1_ptr , DatabaseRecord* DatabaseRecord_ptr);
    ~SingleMs1Match();//析构函数
public:
    Ms1* m_Ms1_ptr;//原始的一级
    DatabaseRecord* m_DatabaseRecord_ptr;//记录的配对结果
};

#endif // SINGLEMS1MATCH_H

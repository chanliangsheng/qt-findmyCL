#ifndef MZML_H
#define MZML_H

#include "ms1.h"
#include "ms2.h"
#include <QObject>
#include <QString>
#include <tinyxml2.h>
#include <QDebug>
#include "base64.h"
#include <zlib.h>
#include <stdlib.h>
#include <iostream>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <memory>


class Mzml: public QObject
{
    Q_OBJECT
public:
    Mzml(QObject *parent = nullptr);
public:
    //所有的ms1和ms2存放的位置
    std::vector<Ms1> m_ms1_vector;
    std::vector<Ms2> m_ms2_vector;

    //数据的编码与压缩
    const char* m_bit_type_param = "32-bit float";
    const char* m_compression_param = "no compression";
public:
    void ReadMzml(QString file_name);//读取mzml文件
    void ParserMs1(tinyxml2::XMLElement *spectrum_node);//解析一级节点
    void ParserMs2(tinyxml2::XMLElement *spectrum_node);//解析一级节点
    char GetMsLevel(tinyxml2::XMLElement *spectrum_node);//获取该节点是属于一级节点还是二级节点
    void GetEncodeCompressionParam(tinyxml2::XMLElement *spectrum_node);//获取数据编码的压缩的方法
    std::shared_ptr<std::vector<float>> BytesToFloat(std::string &byte_array);//转化byte数组成float数组，智能指针
    std::shared_ptr<std::vector<double>> BytesToDouble(std::string &byte_array);//转化byte数组成double数组，智能指针
    std::string ZlibDecompress(const std::string& str);//解压zlib的字符串
};

#endif // MZML_H

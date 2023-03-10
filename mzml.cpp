#include "mzml.h"

Mzml::Mzml(QObject *parent) : QObject(parent)
{

}

void Mzml::ReadMzml(QString file_name)
{
    using namespace tinyxml2;
    XMLDocument xml;
    //载入数据,需要类型是char*
    xml.LoadFile(file_name.toStdString().c_str());

    //根节点
    XMLElement *spectrumList_node = xml.RootElement();

    //如果找到了spectrumList节点，则退出循环，如果没有找到，则提示错误文件
    while(true){
        if(spectrumList_node == nullptr){
            qDebug() << "wrong file!!!";
            return;
        }
       else if(std::string(spectrumList_node->Name()) == "indexedmzML"){
           spectrumList_node = spectrumList_node->FirstChildElement("mzML");
       }
       else if(std::string(spectrumList_node->Name()) == "mzML"){
           spectrumList_node = spectrumList_node->FirstChildElement("run");
       }
       else if(std::string(spectrumList_node->Name()) == "run"){
           spectrumList_node = spectrumList_node->FirstChildElement("spectrumList");
       }
        else if(std::string(spectrumList_node->Name()) == "spectrumList"){
            break;
        }
    }

    //获取压缩方式和字节编码格式
    this->GetEncodeCompressionParam(spectrumList_node->FirstChildElement());

    //遍历所有的spectrum节点，读取信息
    for(XMLElement *spectrum_node = spectrumList_node->FirstChildElement("spectrum") ; spectrum_node != nullptr ; spectrum_node = spectrum_node->NextSiblingElement()){
        char ms_level = this->GetMsLevel(spectrum_node);//获取ms_level信息
        if(ms_level == '1'){
            this->ParserMs1(spectrum_node);
        }
        else if (ms_level == '2') {
            this->ParserMs2(spectrum_node);
        }
    }

    return;
}

void Mzml::ParserMs1(tinyxml2::XMLElement* spectrum_node)
{
    using namespace tinyxml2;

    //获取保留时间，单位为分钟
    const char* rt_char = spectrum_node->FirstChildElement("scanList")->FirstChildElement("scan")->FirstChildElement("cvParam")->Attribute("value");
    float rt = atof(rt_char);

    //获取mz节点和intensity节点
    XMLElement* mz_node = spectrum_node->FirstChildElement("binaryDataArrayList")->FirstChildElement();
    XMLElement* intensity_node = mz_node->NextSiblingElement();

    //base64数据转化成字节数组
    std::string mz_data = base64_decode(mz_node->FirstChildElement("binary")->GetText());
    std::string intensity_data = base64_decode(intensity_node->FirstChildElement("binary")->GetText());



    //解压string
    if(std::string(this->m_compression_param) == "zlib compression"){
        mz_data = this->ZlibDecompress(mz_data);
        intensity_data = this->ZlibDecompress(intensity_data);
    }
    else if(std::string(this->m_compression_param) == "no compression"){

    }


    //从字节数组转化为数组，存储到m_ms1_vector中
    if(std::string(this->m_bit_type_param) == "32-bit float"){
        std::vector<float>* mz_original_data = this->BytesToFloat(mz_data);
        std::vector<float>* intensity_original_data = this->BytesToFloat(intensity_data);
        for(unsigned int i = 0;i < mz_original_data->size() ; i++){
           this->m_ms1_vector.emplace_back(Ms1((*mz_original_data)[i],(*intensity_original_data)[i],rt));
        }
        //清除内存
        delete  mz_original_data;
        delete  intensity_original_data;
    }
    else if(std::string(this->m_bit_type_param) == "64-bit float"){
        std::vector<double>* mz_original_data = this->BytesToDouble(mz_data);
        std::vector<double>* intensity_original_data = this->BytesToDouble(intensity_data);
        for(unsigned int i = 0;i < mz_original_data->size() ; i++){
            this->m_ms1_vector.emplace_back(Ms1((*mz_original_data)[i],(*intensity_original_data)[i],rt));
        }
        //清除内存
        delete  mz_original_data;
        delete  intensity_original_data;
    }
    return;
}

void Mzml::ParserMs2(tinyxml2::XMLElement *spectrum_node)
{
    using namespace tinyxml2;

    //获取保留时间，单位为分钟
    const char* rt_char = spectrum_node->FirstChildElement("scanList")->FirstChildElement("scan")->FirstChildElement("cvParam")->Attribute("value");
    float rt = atof(rt_char);

    //获取前体离子信息
    float precursor_ion_mz;
    float precursor_ion_intensity = 0;
    XMLElement* precursor_ion_node = spectrum_node->FirstChildElement("precursorList");
    //获取参数的节点，获取真实信息
    while (true) {
        if(std::string(precursor_ion_node->Name()) != "cvParam"){
            precursor_ion_node = precursor_ion_node->FirstChildElement();
        }
        else{
            precursor_ion_mz = atof(precursor_ion_node->Attribute("value"));
            break;
        }
    }



    //获取mz节点和intensity节点
    XMLElement* mz_node = spectrum_node->FirstChildElement("binaryDataArrayList")->FirstChildElement();
    XMLElement* intensity_node = mz_node->NextSiblingElement();

    //base64数据转化成字节数组
    std::string mz_data = base64_decode(mz_node->FirstChildElement("binary")->GetText());
    std::string intensity_data = base64_decode(intensity_node->FirstChildElement("binary")->GetText());

    //解压
    if(std::string(this->m_compression_param) == "zlib compression"){
        mz_data = this->ZlibDecompress(mz_data);
        intensity_data = this->ZlibDecompress(intensity_data);
    }
    else if(std::string(this->m_compression_param) == "no compression"){

    }

    //从字节数组转化为数组，存储到m_ms2_vector中
    if(std::string(this->m_bit_type_param) == "32-bit float"){
        std::vector<float>* mz_original_data = this->BytesToFloat(mz_data);
        std::vector<float>* intensity_original_data = this->BytesToFloat(intensity_data);
        this->m_ms2_vector.emplace_back(Ms2(precursor_ion_mz , precursor_ion_intensity , rt , *mz_original_data , *intensity_original_data));
        //清除内存
//        std::vector<float>().swap(*mz_original_data);
//        std::vector<float>().swap(*intensity_original_data);
        delete  mz_original_data;
        delete  intensity_original_data;
    }
    else if(std::string(this->m_bit_type_param) == "64-bit float"){
        std::vector<double>* mz_original_data = this->BytesToDouble(mz_data);
        std::vector<double>* intensity_original_data = this->BytesToDouble(intensity_data);
        this->m_ms2_vector.emplace_back(Ms2(precursor_ion_mz , precursor_ion_intensity , rt , *mz_original_data , *intensity_original_data));
        //清除内存
//        std::vector<double>().swap(*mz_original_data);
//        std::vector<double>().swap(*intensity_original_data);
        delete  mz_original_data;
        delete  intensity_original_data;
    }
    return;
}

char Mzml::GetMsLevel(tinyxml2::XMLElement *spectrum_node)
{
    auto ms_level_node = spectrum_node->FirstChildElement();//找第一个子节点
    while (true) {
        if(std::string(ms_level_node->Attribute("name")) == "ms level"){
            return *(ms_level_node->Attribute("value"));//返回ms_level
        }
        else{
            ms_level_node = ms_level_node->NextSiblingElement();//到下一个节点
            //判断是否溢出
            if(ms_level_node == nullptr){
                return '0';
            }
        }
    }
}

void Mzml::GetEncodeCompressionParam(tinyxml2::XMLElement *spectrum_node)
{
    using namespace tinyxml2;

    //获取存放参数的节点
    XMLElement* parameter_node = spectrum_node->FirstChildElement("binaryDataArrayList")->FirstChildElement("binaryDataArray")->FirstChildElement();

    while (true) {
        if(parameter_node->Attribute("name") == NULL){
            return;
        }
        else if((std::string(parameter_node->Attribute("name")) == "32-bit float") || (std::string(parameter_node->Attribute("name")) == "64-bit float")){
            this->m_bit_type_param = parameter_node->Attribute("name");
        }
        else if((std::string(parameter_node->Attribute("name")) == "no compression") || (std::string(parameter_node->Attribute("name")) == "zlib compression")){
            this->m_compression_param = parameter_node->Attribute("name");
        }
        parameter_node = parameter_node->NextSiblingElement();//下一个节点
    }
}

std::vector<float>* Mzml::BytesToFloat(std::string &byte_array)
{
    std::vector<float>* vector_float = new std::vector<float>;//初始化为空数组
    float output;
    for(unsigned int i = 0 ; i < byte_array.size(); i = i + 4){
        *((uchar*)(&output) + 3) = byte_array[i + 3];
        *((uchar*)(&output) + 2) = byte_array[i + 2];
        *((uchar*)(&output) + 1) = byte_array[i + 1];
        *((uchar*)(&output) + 0) = byte_array[i + 0];
        vector_float->emplace_back(output);
    }
    return vector_float;//返回数组的指针
}

std::vector<double>* Mzml::BytesToDouble(std::string &byte_array)
{
    std::vector<double>* vector_double = new std::vector<double>;//初始化为空数组
    double output;
    for(unsigned int i = 0 ; i < byte_array.size(); i = i + 8){
        *((uchar*)(&output) + 7) = byte_array[i + 7];
        *((uchar*)(&output) + 6) = byte_array[i + 6];
        *((uchar*)(&output) + 5) = byte_array[i + 5];
        *((uchar*)(&output) + 4) = byte_array[i + 4];
        *((uchar*)(&output) + 3) = byte_array[i + 3];
        *((uchar*)(&output) + 2) = byte_array[i + 2];
        *((uchar*)(&output) + 1) = byte_array[i + 1];
        *((uchar*)(&output) + 0) = byte_array[i + 0];
        vector_double->emplace_back(output);
    }
    return vector_double;//返回数组的指针
}

std::string Mzml::ZlibDecompress(const std::string &str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);
        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                             zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

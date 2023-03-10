#include "ms2.h"

Ms2::Ms2()
{

}

Ms2::Ms2(float precursor_ion_mz, float precursor_ion_intensity, float rt, std::vector<float> &fragment_ion_mz, std::vector<float> &fragment_ion_intensity)
{
    //有参构造函数
    this->m_rt = rt;
    this->m_fragment_ion_mz = fragment_ion_mz;
    this->m_precursor_ion_mz = precursor_ion_mz;
    this->m_fragment_ion_intensity = fragment_ion_intensity;
    this->m_precursor_ion_intensity = precursor_ion_intensity;
}

Ms2::Ms2(float precursor_ion_mz, float precursor_ion_intensity, float rt, std::vector<double> &fragment_ion_mz, std::vector<double> &fragment_ion_intensity)
{
    this->m_rt = rt;
    this->m_precursor_ion_mz = precursor_ion_mz;
    this->m_precursor_ion_intensity = precursor_ion_intensity;

    //声明固定长度的vector
    this->m_fragment_ion_mz = std::vector<float> (fragment_ion_mz.size());
    this->m_fragment_ion_intensity = std::vector<float> (fragment_ion_intensity.size());

    //转换为float数组
    std::transform(fragment_ion_mz.begin(), fragment_ion_mz.end(), this->m_fragment_ion_mz.begin(), [=](double x) { return (float)x;});
    std::transform(fragment_ion_intensity.begin(), fragment_ion_intensity.end(), this->m_fragment_ion_intensity.begin(), [=](double x) { return (float)x;});

}

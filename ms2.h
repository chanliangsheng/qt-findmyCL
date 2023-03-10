#ifndef MS2_H
#define MS2_H

#include <vector>
#include <algorithm>

class Ms2
{
public:
    Ms2();
    Ms2(float precursor_ion_mz, float precursor_ion_intensity , float rt , std::vector<float> &fragment_ion_mz , std::vector<float> &fragment_ion_intensity);
    Ms2(float precursor_ion_mz, float precursor_ion_intensity , float rt , std::vector<double> &fragment_ion_mz , std::vector<double> &fragment_ion_intensity);
public:
    //前体离子的信息
    float m_precursor_ion_mz = 0;
    float m_precursor_ion_intensity = 0;
    float m_rt;
    //碎片离子的信息
    std::vector<float> m_fragment_ion_mz;
    std::vector<float> m_fragment_ion_intensity;
};

#endif // MS2_H

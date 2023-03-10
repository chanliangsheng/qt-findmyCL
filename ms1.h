#ifndef MS1_H
#define MS1_H


class Ms1
{
public:
    Ms1();
    Ms1(float mz , float intensity , float rt);
    Ms1(double mz , double intensity , double rt);
    Ms1(double mz , double intensity , float rt);
public:
    float m_mz = 0;
    float m_intensity = 0;
    float m_rt = 0;
    float m_rt_min = 0;
    float m_rt_max = 0;
};

#endif // MS1_H

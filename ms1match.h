#ifndef MS1MATCH_H
#define MS1MATCH_H

#include <singlems1match.h>
#include <utility>
#include <database.h>
#include <map>
#include <QTime>

class Ms1Match
{
public:
    Ms1Match();
public:
    //不同的心磷脂的配对的结果，pair的First存储[M-H]-的指针，Second存储[M-2H]2-的指针
    std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match> , std::shared_ptr<SingleMs1Match>>>> m_cl_match;
    std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match> , std::shared_ptr<SingleMs1Match>>>> m_mlcl_match;
    std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match> , std::shared_ptr<SingleMs1Match>>>> m_dlcl_match;
    float m_ppm = 5;//ppm
    float m_tolerance_rt = 6;//可容忍的时间
public:
    //一级与[M-H]-和[M-2H]2-的库进行配对，返回m_cl_match / m_mlcl_match / m_dlcl_match的智能指针
    std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match>,std::shared_ptr<SingleMs1Match>>>> MatchMs1With2Tables(std::vector<Ms1>& ms1_vector , std::pair<std::vector<DatabaseRecord>& , std::vector<DatabaseRecord>&> database_record_vector_pair);
    void MatchMs1WithAllTables(std::vector<Ms1>& ms1_vector , Database& database);

    //寻找相互匹配的M-H和M-2H
    void MatchM_hWithM_2h(std::multimap<int , Ms1*>& m_h_hash_map , std::multimap<int , Ms1*>& m_h_left_hash_map ,int key , Ms1* ms1_pointer ,std::vector<DatabaseRecord>& m_h_database_record , std::vector<DatabaseRecord>& m_2h_database_record , std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match>,std::shared_ptr<SingleMs1Match>>>>& ret);
};

#endif // MS1MATCH_H

#include "ms1match.h"
#include <map>

using namespace std;
Ms1Match::Ms1Match()
{

}

std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match> , std::shared_ptr<SingleMs1Match>>>> Ms1Match::MatchMs1With2Tables(std::vector<Ms1> &ms1_vector, std::pair<std::vector<DatabaseRecord> &, std::vector<DatabaseRecord> &> database_record_vector_pair)
{
    //std::vector<DatabaseRecord>中的mz已经按照从小到大排序了，所以不需要再排序
    //M-H和M-2H的同一个索引下的心磷脂是相同的，只是加和形式的区别
    vector<DatabaseRecord> m_h_database_record = database_record_vector_pair.first;
    vector<DatabaseRecord> m_2h_database_record = database_record_vector_pair.second;

    //结果ret，智能指针
    shared_ptr<vector<pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>>> ret = make_shared<vector<pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>>>();

    multimap<int , Ms1*> m_h_hash_map;//M-H配对上的结果，多对应哈希表，因为同一个int可能对应多个MS1*
    multimap<int , Ms1*> m_h_left_hash_map;//用来存储仅有M-H，没有M-2H的M-H
    //二分法，先搜索M-H，再搜索M-2H，然后去除同时再M-H的部分

    QTime timedebuge;//声明一个时钟对象
    timedebuge.start();//开始计时
    //对M-H进行搜索
    for(std::vector<Ms1>::iterator ms1_itr = ms1_vector.begin() ; ms1_itr != ms1_vector.end() ; ms1_itr++){
        //左索引和右索引
        int left = 0;
        int right = m_h_database_record.size() - 1;
        while (left < right) {
            int mid = (left + right) / 2;
            //基于ppm所给定的范围，需要样本中的mz位于m_h_min_mz和m_h_max_mz，才认为是符合的
            float m_h_min_mz = m_h_database_record[mid].m_mz - (m_h_database_record[mid].m_mz*(this->m_ppm)/1000000);
            float m_h_max_mz = m_h_database_record[mid].m_mz + (m_h_database_record[mid].m_mz*(this->m_ppm)/1000000);
            //如果符合要求
            if((ms1_itr->m_mz >= m_h_min_mz) && (ms1_itr->m_mz <= m_h_max_mz)){
                Ms1* ms1_pointer = &(*ms1_itr);//迭代器转换为指针
                m_h_hash_map.insert({mid , ms1_pointer});//加入到哈希表中
                unsigned int left_t = mid - 1;
                unsigned int right_t = mid + 1;
                //mid的左边一位和右边一位的各自的符合区间
                float m_h_left_t_min_mz;
                float m_h_left_t_max_mz;
                float m_h_right_t_min_mz;
                float m_h_right_t_max_mz;
                while(left_t >= 0){
                    //更新区间
                    m_h_left_t_min_mz = m_h_database_record[left_t].m_mz - (m_h_database_record[left_t].m_mz*(this->m_ppm)/1000000);
                    m_h_left_t_max_mz = m_h_database_record[left_t].m_mz + (m_h_database_record[left_t].m_mz*(this->m_ppm)/1000000);
                    if((ms1_itr->m_mz >= m_h_left_t_min_mz) && (ms1_itr->m_mz <= m_h_left_t_max_mz)){
                        m_h_hash_map.insert({left_t , ms1_pointer});//加入到哈希表中
                        left_t--;//向左继续找
                    }
                    else{
                        break;
                    }
                }
                while(right_t <= (m_h_database_record.size() - 1)){
                    //更新区间
                    m_h_right_t_min_mz = m_h_database_record[right_t].m_mz + (m_h_database_record[right_t].m_mz*(this->m_ppm)/1000000);
                    m_h_right_t_max_mz = m_h_database_record[right_t].m_mz + (m_h_database_record[right_t].m_mz*(this->m_ppm)/1000000);
                    if((ms1_itr->m_mz >= m_h_right_t_min_mz) && (ms1_itr->m_mz <= m_h_right_t_max_mz)){
                        m_h_hash_map.insert({right_t , ms1_pointer});
                        right_t++;//向右继续寻找
                    }
                    else{
                        break;
                    }
                }

                break;//找完左边和右边的，就说明ms1_itr的元素已经找完了，跳出这个while
            }
            //如果mz大于mid对应的mz，说明在右侧可能有配对的
            else if(ms1_itr->m_mz >= m_h_max_mz){
                left = mid + 1;
            }
            //如果mz小于mid对应的mz，说明在左侧可能有配对的
            else if(ms1_itr->m_mz <= m_h_min_mz){
                right = mid - 1;
            }
        }
    }

    qDebug()<<"第一段程序耗时："<<timedebuge.elapsed()/1000.0<<"s";//输出计时
    m_h_left_hash_map = m_h_hash_map;//赋值，m_h_left_hash_map用来保留那些仅有M-H的

    //对M-2H进行搜索，查找既有M-H又有M-2H的一级，m_h_hash_map去除了既有M-H中有M-2H的部分
    for(vector<Ms1>::iterator ms1_itr = ms1_vector.begin() ; ms1_itr != ms1_vector.end() ; ms1_itr++){
        //左索引和右索引
        int left = 0;
        int right = m_2h_database_record.size() - 1;
        while (left <= right) {
            int mid = (left + right)/2;//中值
            //基于ppm所给定的范围，需要样本中的mz位于m_2h_min_mz和m_2h_max_mz，才认为是符合的
            float m_2h_min_mz = m_2h_database_record[mid].m_mz - (m_2h_database_record[mid].m_mz*(this->m_ppm)/1000000);
            float m_2h_max_mz = m_2h_database_record[mid].m_mz + (m_2h_database_record[mid].m_mz*(this->m_ppm)/1000000);
            Ms1* ms1_pointer;//一级的指针
            if((ms1_itr->m_mz >= m_2h_min_mz) && (ms1_itr->m_mz <= m_2h_max_mz)){
                ms1_pointer = &*ms1_itr;
                //判断哪些M-H跟M-2H是一起的，加入到结果ret中，ret传的是引用，所以会直接在ret上修改
                this->MatchM_hWithM_2h(m_h_hash_map , m_h_left_hash_map , mid , ms1_pointer , m_h_database_record , m_2h_database_record , ret);
                qDebug() << ret->size();
                unsigned int left_t = mid - 1;
                unsigned int right_t = mid + 1;
                //mid的左边一位和右边一位的各自的符合区间
                float m_2h_left_t_min_mz;
                float m_2h_left_t_max_mz;
                float m_2h_right_t_min_mz;
                float m_2h_right_t_max_mz;
                while(left_t >= 0){
                    m_2h_left_t_min_mz = m_2h_database_record[left_t].m_mz - (m_2h_database_record[left_t].m_mz*(this->m_ppm)/1000000);
                    m_2h_left_t_max_mz = m_2h_database_record[left_t].m_mz + (m_2h_database_record[left_t].m_mz*(this->m_ppm)/1000000);
                    if((ms1_itr->m_mz >= m_2h_left_t_min_mz) && (ms1_itr->m_mz <= m_2h_left_t_max_mz)){
                        // this->MatchM_hWithM_2h(m_h_hash_map , m_h_left_hash_map , left_t , ms1_pointer , m_h_database_record , m_2h_database_record , ret);
                        left_t--;//向左继续找
                    }
                    else{
                        break;
                    }
                }

                while (right_t <= m_2h_database_record.size() - 1) {
                    m_2h_right_t_min_mz = m_2h_database_record[right_t].m_mz + (m_2h_database_record[right_t].m_mz*(this->m_ppm)/1000000);
                    m_2h_right_t_max_mz = m_2h_database_record[right_t].m_mz + (m_2h_database_record[right_t].m_mz*(this->m_ppm)/1000000);
                    if((ms1_itr->m_mz >= m_2h_right_t_min_mz) && (ms1_itr->m_mz <= m_2h_right_t_max_mz)){
                        //this->MatchM_hWithM_2h(m_h_hash_map , m_h_left_hash_map , right_t , ms1_pointer , m_h_database_record , m_2h_database_record , ret);
                        right_t++;
                    }
                    else{
                        break;
                    }
                }
            }
            else if(ms1_itr->m_mz >= m_2h_min_mz){
                left = mid + 1;
            }
            else if(ms1_itr->m_mz <= m_2h_max_mz){
                right = mid - 1;
            }
        }
    }

    //遍历哈希表，m_h_left_hash_map仅有M-H，其中既有M-H又有M-2H已经在Ms1Match::MatchM_hWithM_2h被排除了
    for(std::map<int , Ms1*>::iterator itr_map = m_h_left_hash_map.begin(); itr_map != m_h_left_hash_map.end() ; itr_map ++){
        SingleMs1Match m_h(itr_map->second , &m_h_database_record[itr_map->first]);//first指的是mz在m_h_database_record中匹配的索引，second指的是MS1*
        SingleMs1Match m_2h;
        ret->emplace_back(pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>(make_shared<SingleMs1Match>(m_h) , make_shared<SingleMs1Match>(m_2h)));
        ret->end()->second = nullptr;//因为m_2h是没有的，所有指向空指针
    }
    return ret;
}

void Ms1Match::MatchMs1WithAllTables(std::vector<Ms1> &ms1_vector, Database &database)
{
    //配对
    auto cl_match_pointer = this->MatchMs1With2Tables(ms1_vector , pair<vector<DatabaseRecord>&,vector<DatabaseRecord>&>(database.m_CL_M_H,database.m_CL_M_2H));
    auto mlcl_match_pointer = this->MatchMs1With2Tables(ms1_vector , pair<vector<DatabaseRecord>&,vector<DatabaseRecord>&>(database.m_MLCL_M_H,database.m_MLCL_M_2H));
    auto dlcl_match_pointer = this->MatchMs1With2Tables(ms1_vector , pair<vector<DatabaseRecord>&,vector<DatabaseRecord>&>(database.m_DLCL_M_H,database.m_DLCL_M_2H));

    //赋值
    this->m_cl_match = cl_match_pointer;
    this->m_mlcl_match = mlcl_match_pointer;
    this->m_dlcl_match = dlcl_match_pointer;
}

void Ms1Match::MatchM_hWithM_2h(std::multimap<int , Ms1*>& m_h_hash_map , std::multimap<int , Ms1*>& m_h_left_hash_map ,int key , Ms1* ms1_pointer ,std::vector<DatabaseRecord>& m_h_database_record , std::vector<DatabaseRecord>& m_2h_database_record , std::shared_ptr<std::vector<std::pair<std::shared_ptr<SingleMs1Match>,std::shared_ptr<SingleMs1Match>>>>& ret)
{
    auto match_itr =  m_h_hash_map.equal_range(key);//寻找M-H中是否匹配上同一个心磷脂，M-H和M-2H是同一个mid，多重哈希表返回范围
    //如果在M-H中没有找到同一个心磷脂
    if((match_itr.first == m_h_hash_map.end()) && (match_itr.second == m_h_hash_map.end())){
        SingleMs1Match m_h;
        SingleMs1Match m_2h(ms1_pointer , &m_2h_database_record[key]);
        ret->emplace_back(pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>(make_shared<SingleMs1Match>(m_h) , make_shared<SingleMs1Match>(m_2h)));
        ret->back().first = nullptr;
    }
    //如果M-H找到了同一个心磷脂
    else if(match_itr.first == match_itr.second){
        //如果只有一个m_h对应上了，多重哈希表的一个键只对应了一个值
        SingleMs1Match m_h(match_itr.first->second , &m_h_database_record[key]);
        SingleMs1Match m_2h(ms1_pointer , &m_2h_database_record[key]);
        ret->emplace_back(pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>(make_shared<SingleMs1Match>(m_h) , make_shared<SingleMs1Match>(m_2h)));
        //m_h_left_hash_map中删除即有M-H又有M-2H的内容
        auto match_left_itr = m_h_left_hash_map.equal_range(key);
        if((match_left_itr.first == m_h_left_hash_map.end()) && (match_left_itr.second == m_h_left_hash_map.end())){

        }
        else{
            m_h_left_hash_map.erase(match_left_itr.first);//以迭代器的方式来删除元素
        }

    }
    else if(match_itr.first != match_itr.second){
        //如果有多个m_h对应上了
        for(auto itr = match_itr.first ; itr != match_itr.second ; itr++){
            SingleMs1Match m_h(itr->second , &m_h_database_record[key]);
            SingleMs1Match m_2h(ms1_pointer , &m_2h_database_record[key]);
            ret->emplace_back(pair<shared_ptr<SingleMs1Match>,shared_ptr<SingleMs1Match>>(make_shared<SingleMs1Match>(m_h) , make_shared<SingleMs1Match>(m_2h)));
        }
        auto match_left_itr = m_h_left_hash_map.equal_range(key);
        if((match_left_itr.first == m_h_left_hash_map.end()) && (match_left_itr.second == m_h_left_hash_map.end())){

        }
        else if(match_left_itr.first == match_left_itr.second){
            m_h_left_hash_map.erase(match_left_itr.first);//以迭代器的方式来删除元素
        }
        else if(match_left_itr.first != match_left_itr.second){
            for(auto itr = match_left_itr.first;itr!=match_left_itr.second;itr++){
                m_h_left_hash_map.erase(itr);//以迭代器的方式来删除元素
            }
        }
    }

}

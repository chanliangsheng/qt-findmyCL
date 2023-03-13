#include "singlems1match.h"


using namespace std;
SingleMs1Match::SingleMs1Match()
{

}


SingleMs1Match::SingleMs1Match(Ms1 *Ms1_ptr, DatabaseRecord *DatabaseRecord_ptr)
{
    this->m_Ms1_ptr = Ms1_ptr;
    this->m_DatabaseRecord_ptr = DatabaseRecord_ptr;
}


SingleMs1Match::~SingleMs1Match()
{

}




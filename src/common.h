#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstdint>
#include <string>
#include "date.h"

namespace SAM {

struct CourseInfo
{
    typedef std::string IDType;  // allow course ID to have letters

    IDType id;
    std::string name;
    int credit;

    Date begin_date;
    Date end_date;

    std::string teacher_name;
};

struct StudentInfo
{
    typedef std::uint_least64_t IDType;

    IDType id;
    std::string name;
    bool is_male;
};

struct ExamInfo
{
    typedef double ScoreType;

    CourseInfo::IDType course_id;
    std::string name;
    Date date;
};

}  // namespace SAM

#endif  // SAM_COMMON_H_

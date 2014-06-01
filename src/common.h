#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstdint>
#include <string>
#include "date.h"

namespace SAM {

struct BasicCourseInfo
{
    typedef std::uint_least64_t IDType;

    std::string name;
    IDType id;

    Date begin_date;
    Date end_date;

    std::string teacher_name;
};

struct BasicStudentInfo
{
    typedef std::uint_least64_t IDType;

    std::string name;
    IDType id;
    bool is_male;
};

struct BasicExamInfo
{
    std::string name;
    Date date;
};

}  // namespace SAM

#endif  // SAM_COMMON_H_

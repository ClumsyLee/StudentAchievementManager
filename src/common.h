#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstdint>
#include <string>

namespace SAM {

struct Date
{
    int year;
    int month;
    int day;
};

struct CourseInfo
{
    typedef std::string IDType;  // allow course ID to have letters

    IDType id;
    std::string name;
    int credit;

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

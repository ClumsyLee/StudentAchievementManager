#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace SAM {

extern const int kDepartmentNum;
extern const char *kDepartmentName[];

struct CourseInfo
{
    typedef std::string IDType;  // allow course ID to have letters

    IDType id;
    std::string name;
    int department;
    int credit;
    std::size_t capacity;

    std::string teacher_name;
};


struct StudentInfo
{
    typedef std::uint_least64_t IDType;

    IDType id;
    std::string name;
    bool is_male;
    int department;
};



typedef float ScoreType;
const ScoreType kInvalidScore = -1.0f;

struct ScorePiece
{
    StudentInfo::IDType id;
    ScoreType score;
};

typedef std::vector<ScorePiece> FinalScore;

bool MakeCourseInfo(const std::string &str, CourseInfo &info);
std::string to_string(const CourseInfo &info);
bool MakeStudentInfo(const std::string &str, StudentInfo &info);
std::string to_string(const StudentInfo &info);

std::ostream & PrintChinese(std::ostream &os, const std::string &str,
                            std::size_t width);
std::ostream & PrintScore(std::ostream &os, ScoreType score);

std::ostream & ShowDepartments(std::ostream &os);

}  // namespace SAM

#endif  // SAM_COMMON_H_

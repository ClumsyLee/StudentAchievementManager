#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace SAM {

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

typedef float ScoreType;
const ScoreType kInvalidScore = -1.0f;

struct ScorePiece
{
    StudentInfo::IDType id;
    ScoreType score;
};
typedef std::vector<ScorePiece> FinalScore;

}  // namespace SAM

#endif  // SAM_COMMON_H_

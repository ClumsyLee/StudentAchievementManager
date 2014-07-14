#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstdint>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace SAM {

struct CourseInfo
{
    typedef std::string IDType;  // allow course ID to have letters

    CourseInfo() = default;
    explicit CourseInfo(const std::string &line)
    {
        std::istringstream iss(line);
        iss >> id >> name >> credit >> capacity >> teacher_name;
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << id << ' '
            << name << ' '
            << credit << ' '
            << capacity << ' '
            << teacher_name << ' ';
        return oss.str();
    }

    IDType id;
    std::string name;
    int credit;
    int capacity;

    std::string teacher_name;
};

struct StudentInfo
{
    typedef std::uint_least64_t IDType;

    StudentInfo() = default;
    explicit StudentInfo(const std::string &line)
    {
        std::istringstream iss(line);
        iss >> id >> name >> is_male;
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << id << ' '
            << name << ' '
            << is_male << ' ';
        return oss.str();
    }

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

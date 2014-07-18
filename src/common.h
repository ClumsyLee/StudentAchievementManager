#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstddef>
#include <cstdint>

#include <iomanip>
#include <sstream>
#include <string>
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
            << teacher_name;
        return oss.str();
    }

    std::string Format() const
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width + 1) << id << ' '  // 1 Chinese char
            << setw(name_width + name.size() / 3) << name << ' '
            << setw(credit_width) << credit << ' '
            << setw(capacity_width) << capacity << ' '
            << setw(capacity_width + teacher_name.size() / 3) << teacher_name;

        return oss.str();
    }

    static std::string Heading()
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 3) << "课程名" << ' '
            << setw(credit_width + 2) << "学分" << ' '
            << setw(capacity_width + 3) << "课容量" << ' '
            << setw(capacity_width + 2) << "教师";

        return oss.str();
    }

    constexpr static int id_width = 12;
    constexpr static int name_width = 12;
    constexpr static int credit_width = 12;
    constexpr static int capacity_width = 12;
    constexpr static int teacher_name_width = 12;

    IDType id;
    std::string name;
    int credit;
    std::size_t capacity;

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
            << is_male;
        return oss.str();
    }

    std::string Format() const
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << id << ' '
            << setw(name_width + name.size() / 3) << name << ' '
            << setw(is_male_width + 1) << (is_male ? "男" : "女");

        return oss.str();
    }

    static std::string Heading()
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 2) << "姓名" << ' '
            << setw(is_male_width + 2) << "性别";

        return oss.str();
    }

    constexpr static int id_width = 12;
    constexpr static int name_width = 12;
    constexpr static int is_male_width = 12;

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

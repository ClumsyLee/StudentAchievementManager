#ifndef SAM_COMMON_H_
#define SAM_COMMON_H_

#include <cstddef>
#include <cstdint>
#include <cstring>

#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace SAM {

static const int kDepartmentNum = 46;
static const char *kDepartmentName[kDepartmentNum] = {
    "建筑学院",  // 0
    "建筑系",
    "土木工程系",
    "水利水电工程系",
    "环境学院",
    "机械工程学院",  // 5
    "机械工程系",
    "精密仪器系",
    "热能工程系",
    "汽车工程系",
    "工业工程系",  // 10
    "信息科学技术学院",
    "电机工程与应用电子技术系",
    "电子工程系",
    "计算机科学与技术系",
    "自动化系",  // 15
    "微电子与纳电子学系",
    "航天航空学院",
    "工程物理系",
    "工程力学系",
    "化学工程系",  // 20
    "材料学院",
    "理学院",
    "数学科学系",
    "物理系",
    "化学系",  // 25
    "生命科学学院",
    "交叉信息研究院",
    "经济管理学院",
    "人文社会科学学院",
    "中国语言文学系",  // 30
    "外国语言文学系",
    "法学院",
    "新闻与传播学院",
    "人文学院",
    "社会科学学院",  // 35
    "体育教研部",
    "计算机与信息管理中心",
    "美术学院",
    "建设管理系",
    "建筑技术科学系",  // 40
    "微电子学研究所",
    "应用技术学院",
    "医学院",
    "生物医学工程系",
    "软件学院"  // 45
};

struct CourseInfo
{
    typedef std::string IDType;  // allow course ID to have letters

    CourseInfo() = default;
    explicit CourseInfo(const std::string &line)
    {
        std::istringstream iss(line);
        iss >> id >> name >> department >> credit >> capacity >> teacher_name;
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << id << ' '
            << name << ' '
            << department << ' '
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
            << setw(department_width +
                    std::strlen(kDepartmentName[department]) / 3)
                    << kDepartmentName[department] << ' '
            << setw(credit_width) << credit << ' '
            << setw(capacity_width) << capacity << ' '
            << setw(teacher_name_width + teacher_name.size() / 3)
                    << teacher_name;

        return oss.str();
    }

    static std::string Heading()
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 3) << "课程名" << ' '
            << setw(department_width + 2) << "院系" << ' '
            << setw(credit_width + 2) << "学分" << ' '
            << setw(capacity_width + 3) << "课容量" << ' '
            << setw(teacher_name_width + 2) << "教师";

        return oss.str();
    }

    constexpr static int id_width = 12;
    constexpr static int name_width = 12;
    constexpr static int department_width = 16;
    constexpr static int credit_width = 6;
    constexpr static int capacity_width = 7;
    constexpr static int teacher_name_width = 8;

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

    StudentInfo() = default;
    explicit StudentInfo(const std::string &line)
    {
        std::istringstream iss(line);
        iss >> id >> name >> is_male >> department;
    }

    std::string ToString() const
    {
        std::ostringstream oss;
        oss << id << ' '
            << name << ' '
            << is_male << ' '
            << department;
        return oss.str();
    }

    std::string Format() const
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << id << ' '
            << setw(name_width + name.size() / 3) << name << ' '
            << setw(is_male_width + 1) << (is_male ? "男" : "女")
            << setw(department_width +
                    std::strlen(kDepartmentName[department]) / 3)
                    << kDepartmentName[department] << ' ';

        return oss.str();
    }

    static std::string Heading()
    {
        using std::setw;

        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 2) << "姓名" << ' '
            << setw(is_male_width + 2) << "性别"
            << setw(department_width + 2) << "院系";

        return oss.str();
    }

    constexpr static int id_width = 12;
    constexpr static int name_width = 12;
    constexpr static int is_male_width = 12;
    constexpr static int department_width = 16;

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

}  // namespace SAM

#endif  // SAM_COMMON_H_

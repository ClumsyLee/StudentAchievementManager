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

    IDType id;
    std::string name;
    int department;
    int credit;
    std::size_t capacity;

    std::string teacher_name;
};

bool MakeCourseInfo(const std::string &str, CourseInfo &info)
{
    std::istringstream iss(str);
    if (iss >> info.id >> info.name >> info.department >> info.credit
            >> info.capacity >> info.teacher_name)
        return true;
    else
        return false;
}

std::string to_string(const CourseInfo &info)
{
    std::ostringstream oss;
    oss << info.id << ' '
        << info.name << ' '
        << info.department << ' '
        << info.credit << ' '
        << info.capacity << ' '
        << info.teacher_name;
    return oss.str();
}


struct StudentInfo
{
    typedef std::uint_least64_t IDType;

    IDType id;
    std::string name;
    bool is_male;
    int department;
};

bool MakeStudentInfo(const std::string &str, StudentInfo &info)
{
    std::istringstream iss(str);
    if (iss >> info.id >> info.name >> info.is_male >> info.department)
        return true;
    else
        return false;
}

std::string to_string(const StudentInfo &info)
{
    std::ostringstream oss;
    oss << info.id << ' '
        << info.name << ' '
        << info.is_male << ' '
        << info.department;
    return oss.str();
}

std::ostream & PrintChinese(std::ostream &os, const std::string &str,
                            std::size_t width)
{
    os.width(width + str.size() / 3);  // assume every character takes 3 bytes
    os << str;
    return os;
}

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

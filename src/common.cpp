#include "common.h"

namespace SAM {

const int kDepartmentNum = 46;
const char *kDepartmentName[kDepartmentNum] = {
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

std::ostream & PrintScore(std::ostream &os, ScoreType score)
{
    if (score == kInvalidScore)
        os << '*';
    else
        os << score;

    return os;
}

std::ostream & ShowDepartments(std::ostream &os)
{
    for (int index = 0; index < kDepartmentNum; index++)
    {
        os.width(2);
        os << index << ": ";
        os.width(30);
        os << kDepartmentName[index];

        if (index % 2 == 0)
            os << "     ";
        else
            os << std::endl;
    }
    if (kDepartmentNum % 2 == 1)
        os << std::endl;

    return os;
}



}  // namespace SAM

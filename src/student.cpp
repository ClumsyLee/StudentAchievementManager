#include <algorithm>
#include <iomanip>
#include "student.h"

namespace SAM {

Student::Student(const StudentInfo& info)
        : info_(info),
          courses_taken_()
{
}

void Student::AddCourse(const CourseInfo::IDType &course_id)
{
    courses_taken_.push_back(course_id);
    auto range_pair = std::equal_range(courses_taken_.begin(),
                                       courses_taken_.end(),
                                       course_id);
    if (range_pair.first == range_pair.second)  // a new course to take
        courses_taken_.insert(range_pair.first, course_id);
    // else this course already taken, do nothing
}

void Student::RemoveCourse(const CourseInfo::IDType &course_id)
{
    auto range_pair = std::equal_range(courses_taken_.begin(),
                                       courses_taken_.end(),
                                       course_id);
    if (range_pair.first == range_pair.second)  // has not taken this course
        return;

    courses_taken_.erase(range_pair.first);
}

bool Student::InCourse(const CourseInfo::IDType &course_id) const
{
    return std::binary_search(courses_taken_.begin(),
                              courses_taken_.end(),
                              course_id);
}

std::string Student::Heading()
{
    using std::setw;

    static std::string heading;
    static bool heading_is_valid = false;

    if (!heading_is_valid)
    {
        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 2) << "姓名" << ' '
            << setw(is_male_width + 2) << "性别" << ' '
            << setw(department_width + 2) << "院系";
        heading = oss.str();
        heading_is_valid = true;
    }

    return heading;
}

std::ostream & operator<<(std::ostream &os, const Student &student)
{
    using std::setw;
    const StudentInfo &info = student.info();

    os << setw(Student::id_width) << info.id << ' ';
    PrintChinese(os, info.name, Student::name_width) << ' ';
    os << setw(Student::is_male_width + 1) << (info.is_male ? "男" : "女") << ' ';
    PrintChinese(os, kDepartmentName[info.department], Student::department_width);

    return os;
}

}  // namespace SAM

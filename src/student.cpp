#include <algorithm>
#include "student.h"

namespace SAM {

Student::Student(const BasicStudentInfo& basic_info)
        : basic_info_(basic_info),
          courses_taken_(),
          courses_is_sorted_(true)
{
}

void Student::AddCourse(BasicCourseInfo::IDType course_id)
{
    courses_taken_.push_back(course_id);
    courses_is_sorted_ = false;
}

void Student::RemoveCourse(BasicCourseInfo::IDType course_id)
{
    courses_taken_.erase(std::remove(courses_taken_.begin(),
                                     courses_taken_.end(),
                                     course_id),
                         courses_taken_.end());
}

bool Student::InCourse(BasicCourseInfo::IDType course_id) const
{
    SortCoursesIfNeeded();

    return std::binary_search(courses_taken_.begin(),
                              courses_taken_.end(),
                              course_id);
}

void Student::SortCoursesIfNeeded() const
{
    if (!courses_is_sorted_)
    {
        std::sort(courses_taken_.begin(), courses_taken_.end());
        courses_taken_.erase(std::unique(courses_taken_.begin(),
                                       courses_taken_.end()),
                             courses_taken_.end());
        courses_is_sorted_ = true;
    }
}



}  // namespace SAM

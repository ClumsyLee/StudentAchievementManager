#include <algorithm>
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


}  // namespace SAM

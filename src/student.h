#ifndef SAM_STUDENT_H_
#define SAM_STUDENT_H_

#include <string>
#include <vector>

#include "common.h"

namespace SAM {

// Store the basic information of a student, and manage the course he/she takes.
// Every student should has a UNIQUE id.
class Student
{
 public:
    typedef BasicStudentInfo::IDType IDType;

    Student() = default;
    explicit Student(const BasicStudentInfo& basic_info);

    void AddCourse(BasicCourseInfo::IDType course_id);
    void RemoveCourse(BasicCourseInfo::IDType course_id);
    bool InCourse(BasicCourseInfo::IDType course_id) const;

    // accessors
    const BasicStudentInfo & basic_info() const { return basic_info_; }
    const std::vector<BasicCourseInfo::IDType> &courses_taken() const
    { return courses_taken_; }

    // mutators
    // ID is ought to be unique, so remember to check whether there is
    // a student with a same id, and update the id that courses have
    void set_basic_info(const BasicStudentInfo &info) { basic_info_ = info; }

 private:
    void SortCoursesIfNeeded() const;  // to make it possible to call InCourse
                                       // on a const object

    BasicStudentInfo basic_info_;

    mutable std::vector<BasicCourseInfo::IDType> courses_taken_;
    mutable bool courses_is_sorted_;
};

}  // namespace SAM

#endif  // SAM_STUDENT_H_

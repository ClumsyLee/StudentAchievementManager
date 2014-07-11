#ifndef SAM_STUDENT_H_
#define SAM_STUDENT_H_

#include <string>
#include <vector>

#include "common.h"

namespace SAM {

// Store the basic information of a student, and manage the course he/she takes.
// Every student shall has a UNIQUE id.
class Student
{
 public:
    typedef StudentInfo::IDType IDType;

    Student() = default;
    explicit Student(const StudentInfo& info);

    void AddCourse(const CourseInfo::IDType &course_id);
    void RemoveCourse(const CourseInfo::IDType &course_id);
    bool InCourse(const CourseInfo::IDType &course_id) const;

    // accessors
    const StudentInfo & info() const { return info_; }
    const std::vector<CourseInfo::IDType> & courses_taken() const
    { return courses_taken_; }

    // mutators
    // ID is ought to be unique, so remember to check whether there is
    // a student with a same id, and update the id that courses have
    void set_info(const StudentInfo &info) { info_ = info; }

 private:
    StudentInfo info_;

    std::vector<CourseInfo::IDType> courses_taken_;  // always sorted
};

}  // namespace SAM

#endif  // SAM_STUDENT_H_

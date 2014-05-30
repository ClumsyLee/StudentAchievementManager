#ifndef SAM_STUDENT_H_
#define SAM_STUDENT_H_

#include <cstdint>
#include <string>
#include <vector>
#include "course.h"

namespace SAM {

// Store the basic information of a student, and manage the course he/she takes.
// Every student should has a UNIQUE id.
class Student
{
 public:
    typedef std::uint_least64_t IDType;

    Student(const std::string &name, IDType id, bool is_male);

    void AddCourse(Course::IDType course_id);
    bool RemoveCourse(Course::IDType course_id) const;
    bool InCourse(Course::IDType course_id) const;

    // accessors
    const std::string & name() const { return name_; }
    IDType id() const { return id_; }
    const std::vector<Course::IDType> &courses_taken() const
    { return courses_taken_; }

    // mutators
    Student & set_name(const std::string &name);
    Student & set_id(IDType id);  // Ought to be unique, so remember to check
                                  // whether there is a student with a same id,
                                  // and update the id that courses have

 private:
    std::string name_;
    IDType id_;
    bool is_male_;
    std::vector<Course::IDType> courses_taken_;
};

}  // namespace SAM

#endif  // SAM_STUDENT_H_

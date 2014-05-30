#ifndef SAM_MANAGER_H_
#define SAM_MANAGER_H_

#include <string>
#include <vector>

#include "student.h"
#include "course.h"

namespace SAM {

class Manager
{
 public:
    Manager();

    void AddStudent(const std::string &name, Student::IDType student_id);
    void RemoveStudent(Student::IDType student_id);
    bool HasStudent(Student::IDType student_id) const;

    void AddCourse(const std::string &name, Course::IDType course_id);
    void RemoveCourse(Course::IDType course_id);
    bool HasCourse(Course::IDType course_id) const;

 private:
    // Here we use sorted vector instead of map or something, because students
    // and course are usually added at the beginning, and will stay the same
    // for the next semester. So a sorted vector will be more efficient.
    std::vector<Student> students_;
    std::vector<Course> v;
};

}  // namespace SAM

#endif  // SAM_MANAGER_H_

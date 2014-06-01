#ifndef SAM_MANAGER_H_
#define SAM_MANAGER_H_

#include <map>
#include <string>
#include <vector>

#include "student.h"
#include "course.h"

namespace SAM {

// Manage students and courses.
// Every student/course should has a unique id.
class Manager
{
 public:
    Manager();

    // ======================= Operations for students =======================
    bool AddStudent(const BasicStudentInfo &student_info);
    bool RemoveStudent(Student::IDType student_id);
    bool HasStudent(Student::IDType student_id) const;
    const Student * SearchStudent(Student::IDType student_id) const;

    // IDs that courses have will be updated if needed.
    // If the new ID has been taken, nothing will be changed.
    bool SetStudentBasicInfo(Student::IDType student_id,
                             const BasicStudentInfo &info);


    // ======================= Operations for courses =======================
    bool AddCourse(const BasicCourseInfo &info);
    bool RemoveCourse(Course::IDType course_id);
    bool HasCourse(Course::IDType course_id) const;
    const Course * SearchCourse(Course::IDType course_id) const;

    // IDs that courses have will be updated if needed.
    // If the new ID has been taken, nothing will be changed.
    bool SetCourseBasicInfo(Course::IDType course_id,
                            const BasicCourseInfo &info);


    // ================== Operations for students & courses ==================
    bool AddStudentToCourse(Student::IDType student_id,
                            Course::IDType course_id);
    bool AddStudentToCourse(const std::vector<Student::IDType> &student_ids,
                             Course::IDType course_id);
    bool RemoveStudentFromCourse(Student::IDType student_id,
                                 Course::IDType course_id);

    // ======================== Operations for exams ========================
    // Add an exam result to the record.
    // If the course_id do not match, the function will do nothing.
    // If a student cannot be found in the result, his ID will be added to
    // the back of unscored_students, and his grade will be set to 0.
    bool AddExam(const Exam &exam,
                 std::vector<Student::IDType> &unscored_students);
    // Remove a certain exam from the course
    // If the index is invalid, nothing will be done
    bool RemoveExam(Course::IDType course_id, std::size_t exam_index);

    bool ModifyScore(Student::IDType student_id, Course::IDType course_id,
                     std::size_t exam_index, Exam::ScoreType new_score);

 private:
    // Here we use sorted vector instead of map or something, because students
    // and courses are usually added at the beginning, and will stay the same
    // for the next semester. So a sorted vector will be more efficient.
    std::map<Student::IDType, Student> students_;
    std::map<Course::IDType, Course> courses_;
};

}  // namespace SAM

#endif  // SAM_MANAGER_H_

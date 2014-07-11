
#ifndef SAM_COURSE_H_
#define SAM_COURSE_H_

#include <string>
#include <vector>

#include "common.h"
#include "exam.h"
#include "student.h"

namespace SAM {

// Store the basic information of a course, and manage its exam results.
// id shall be UNIQUE, so a same class in different semester should have
// different id.
class Course
{
 public:
    typedef CourseInfo::IDType IDType;

    Course() = default;
    explicit Course(const CourseInfo &info);

    // Add an exam result to the record.
    // If the course_id do not match, the function will do nothing.
    // If a student cannot be found in the result, his ID will be added to
    // the back of unscored_students, and his grade will be set to 0.
    // If a student has more than one score, the first one will be taken
    void AddExam(const Exam &exam_result,
                 std::vector<Student::IDType> &unscored_students);
    // Remove a certain exam from the course
    // If the index is invalid, nothing will be done
    bool RemoveExam(std::size_t exam_index);


    // The course-taking information will be updated after calling these.
    // For any exam that is taken before, the new student will get a zero.
    void AddStudent(Student &student);
    void RemoveStudent(Student &student);
    // Check whether a certain student is in the course.
    bool HasStudent(Student::IDType student_id) const;
    bool HasStudent(const Student &student) const
    { return HasStudent(student.info().id); }


    // If the arguments are invalid, 0 will be returned.
    Exam::ScoreType LookUpScore(Student::IDType student_id,
                                std::size_t exam_index) const;
    Exam::ScoreType LookUpScore(const Student &student,
                                std::size_t exam_index) const
    { return LookUpScore(student.info().id, exam_index); }
    // Lookup all the scores
    // If the student is not in this course, an empty vector will be returned.
    std::vector<Exam::ScoreType> LookUpScore(Student::IDType student_id) const;
    std::vector<Exam::ScoreType> LookUpScore(const Student &student) const
    { return LookUpScore(student.info().id); }

    // If the arguments are invalid, nothing will be done
    bool ModifyScore(Student::IDType student_id, std::size_t exam_index,
                     Exam::ScoreType new_score);


    // accessors
    const CourseInfo & info() const { return info_; }
    std::vector<Student::IDType> StudentList() const;
    const std::vector<ExamInfo> & exams_info() const { return exams_info_; }

    // mutators
    void set_info(const CourseInfo &info) { info_ = info; }

 private:
    struct StudentInfo
    {
        Student::IDType id;
        std::vector<Exam::ScoreType> scores;

        bool operator==(const StudentInfo &rhs) const { return id == rhs.id; }
        bool operator<(const StudentInfo &rhs) const { return id < rhs.id; }
    };

    // If no matches, null pointer will be returned.
    StudentInfo * FindStudent(Student::IDType student_id);
    const StudentInfo * FindStudent(Student::IDType student_id) const;

    CourseInfo info_;

    std::vector<ExamInfo> exams_info_;
    std::vector<StudentInfo> students_info_;  // always sorted
};

inline Course::StudentInfo * Course::FindStudent(Student::IDType student_id)
{
    return const_cast<StudentInfo *>(
                static_cast<const Course *>(this)->FindStudent(student_id));
}

}  // namespace SAM

#endif  // SAM_COURSE_H_

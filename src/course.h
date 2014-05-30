#ifndef SAM_COURSE_H_
#define SAM_COURSE_H_

#include <cstdint>
#include <string>
#include <vector>
#include "date.h"
#include "student.h"
#include "exam.h"

namespace SAM {

// Store the basic information of a course, and manage its exam results.
// Different course can share the same id.
class Course
{
 public:
    typedef std::uint_least32_t IDType;

    Course(const std::string &name, IDType id);
    Course(const std::string &name, IDType id, const std::string &teacher_name);
    Course(const std::string &name, IDType id, Date begin_date, Date end_date);
    Course(const std::string &name, IDType id, Date begin_date, Date end_date,
           const std::string &teacher_name);

    // Add an exam result to the record.
    // If the course_id do not match, the function will do nothing.
    // If a student cannot be found in the result, his ID will be added to
    // the back of unscored_students, and his grade will be set to 0.
    void AddExam(const Exam &exam,
                 std::vector<Student::IDType> &unscored_students);
    // Remove a certain exam from the course
    // If the index is invalid, nothing will be done
    void RemoveExam(std::size_t exam_index);


    // The course-taking information will be updated after calling these.
    void AddStudent(Student &student);
    void RemoveStudent(Student &student);
    // Check whether a certain student is in the course.
    bool HasStudent(Student::IDType student_id) const;
    bool HasStudent(const Student &student) const
    { return HasStudent(student.id()); }


    // If the arguments are invalid, 0 will be returned.
    Exam::ScoreType LookUpScore(Student::IDType student_id,
                                std::size_t exam_index) const;
    Exam::ScoreType LookUpScore(const Student &student,
                                std::size_t exam_index) const
    { return LookUpScore(student.id(), exam_index); }
    // Lookup all the scores
    // If the student is not in this course, an empty vector will be returned.
    std::vector<Exam::ScoreType> LookUpScore(Student::IDType student_id) const;
    std::vector<Exam::ScoreType> LookUpScore(const Student &student) const
    { return LookUpScore(student.id()); }

    // If the arguments are invalid, nothing will be done
    void ModifyScore(Student::IDType student_id, std::size_t exam_index);


    // accessors
    const std::string & name() const { return name_; }
    IDType id() const { return id_; }
    const Date & begin_date() const { return begin_date_; }
    const Date & end_date() const { return end_date_; }
    const std::string & teacher_name() const { return teacher_name_; }
    const std::vector<ExamInfo> & exams_info() const { return exams_info_; }

    // mutators
    Course & set_name(const std::string &name);
    Course & set_id(IDType id);  // Remember to update ids that students have.
    Course & set_begin_date(const Date &date);
    Course & set_end_date(const Date &date);
    Course & set_teacher_name(const std::string &name);



 private:
    struct StudentInfo
    {
        Student::IDType id;
        std::vector<Exam::ScoreType> scores;
    };

    std::string name_;
    IDType id_;

    Date begin_date_;
    Date end_date_;

    std::string teacher_name_;

    std::vector<ExamInfo> exams_info_;
    std::vector<StudentInfo> students_info_;
};

}  // namespace SAM

#endif  // SAM_COURSE_H_

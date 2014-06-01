#include <algorithm>
#include "course.h"

namespace SAM {

Course::Course(const BasicCourseInfo &basic_info)
        : basic_info_(basic_info),
          exams_info_(),
          students_info_(),
          students_is_sorted_(true)
{
}

void Course::AddExam(const Exam &exam_result,
                     std::vector<Student::IDType> &unscored_students)
{
    if (exam_result.course_id != basic_info_.id)
        return;

    SortStudentsIfNeeded();

    exams_info_.push_back(exam_result.info);  // record exam info

    std::vector<bool> score_taken(students_info_.size(), false);
    for (const auto &result : exam_result.result)
    {
        StudentInfo *p_student = FindStudent(result.first);
        if (p_student != nullptr)  // found
        {
            std::size_t index = p_student - &students_info_[0];
            if (score_taken[index] == false)  // haven't record for this student
            {
                p_student->scores.push_back(result.second);
                score_taken[index] = true;
            }
        }
    }

    for (std::size_t index = 0; index < students_info_.size(); index++)
    {
        if (score_taken[index] == false)
        {
            // no score given, push a zero
            students_info_[index].scores.push_back(0);
            unscored_students.push_back(students_info_[index].id);
        }
    }
}

bool Course::RemoveExam(std::size_t exam_index)
{
    if (exam_index >= exams_info_.size())
        return false;  // invalid exam index

    exams_info_.erase(exams_info_.begin() + exam_index);  // erase exam info
    for (StudentInfo &info : students_info_)  // erase scores
    {
        info.scores.erase(info.scores.begin() + exam_index);
    }
    return true;
}

void Course::AddStudent(Student &student)
{
    student.AddCourse(basic_info_.id);
    students_info_.push_back(
            StudentInfo{student.basic_info().id,
                        std::vector<Exam::ScoreType>(exams_info_.size(), 0)});
    students_is_sorted_ = false;
}

void Course::RemoveStudent(Student &student)
{
    student.RemoveCourse(basic_info_.id);
    students_info_.erase(std::remove(students_info_.begin(),
                                     students_info_.end(),
                                     StudentInfo{student.basic_info().id, {}}),
                         students_info_.end());
    // The sorting state remain the same.
}

bool Course::HasStudent(Student::IDType student_id) const
{
    return FindStudent(student_id) != nullptr;
}

Exam::ScoreType Course::LookUpScore(Student::IDType student_id,
                                    std::size_t exam_index) const
{
    const StudentInfo * p_student = FindStudent(student_id);
    if (p_student == nullptr || exam_index >= exams_info_.size())
    {
        return 0;  // invalid arguments
    }
    else
    {
        return p_student->scores[exam_index];
    }
}

std::vector<Exam::ScoreType>
Course::LookUpScore(Student::IDType student_id) const
{
    const StudentInfo * p_student = FindStudent(student_id);
    if (p_student == nullptr)
    {
        return std::vector<Exam::ScoreType>();  // student not found
    }
    else
    {
        return p_student->scores;
    }
}

bool Course::ModifyScore(Student::IDType student_id, std::size_t exam_index,
                         Exam::ScoreType new_score)
{
    StudentInfo *p_student = FindStudent(student_id);
    if (p_student != nullptr && exam_index < exams_info_.size())
    {
        p_student->scores[exam_index] = new_score;
        return true;
    }
    return false;
}

std::vector<Student::IDType> Course::StudentList() const
{
    std::vector<Student::IDType> student_list;
    for (const StudentInfo &info : students_info_)
    {
        student_list.push_back(info.id);
    }
    return student_list;
}



void Course::SortStudentsIfNeeded() const
{
    if (!students_is_sorted_)
    {
        // sort students by their ids.
        // we use stable sort to ensure that, if a student has been added
        // to this course twise, only scores of earlier one will be kept.
        std::stable_sort(students_info_.begin(), students_info_.end());

        students_info_.erase(
                std::unique(students_info_.begin(),
                            students_info_.end()),
                students_info_.end());  // remove students with the same id.

        students_is_sorted_ = true;
    }
}

Course::StudentInfo * Course::FindStudent(Student::IDType student_id) const
{
    SortStudentsIfNeeded();

    auto range_pair = std::equal_range(students_info_.begin(),
                                       students_info_.end(),
                                       StudentInfo{student_id, {}});
    if (range_pair.first != range_pair.second)  // found
        return &(*range_pair.first);
    else
        return nullptr;
}

}  // namespace SAM

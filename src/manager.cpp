#include "manager.h"

namespace SAM {

Manager::Manager() : students_(),
                     courses_()
{
}

bool Manager::AddStudent(const StudentInfo &student_info)
{
    if (students_.count(student_info.id) != 0)  // id has been occupied
        return false;

    students_.emplace(student_info.id, Student(student_info));
    return true;
}

bool Manager::RemoveStudent(Student::IDType student_id)
{
    auto iter = students_.find(student_id);
    if (iter == students_.end())  // student not exist
        return false;

    auto  courses_taken = iter->second.courses_taken();
    for (Course::IDType course_id : courses_taken)
    {
        // Courses are managed by Manager, so this course should exist
        courses_[course_id].RemoveStudent(iter->second);
    }
    students_.erase(iter);
    return true;
}

bool Manager::HasStudent(Student::IDType student_id) const
{
    return students_.count(student_id) != 0;
}

Manager::StudentIterator Manager::FindStudent(Student::IDType student_id) const
{
    return StudentIterator(students_.find(student_id));
}

bool Manager::SetStudentInfo(Student::IDType student_id,
                             const StudentInfo &info)
{
    auto iter = students_.find(student_id);
    if (iter == students_.end())
        return false;

    if (student_id != info.id)  // the id has changed
    {
        if (students_.count(info.id) != 0)  // id already been taken
            return false;

        Student new_student(info);
        // updating IDs
        auto courses_taken = iter->second.courses_taken();
        for (Course::IDType course_id : courses_taken)
        {
            // save scores
            auto score_before = courses_[course_id].GetScore(student_id);

            courses_[course_id].RemoveStudent(iter->second);
            courses_[course_id].AddStudent(new_student);

            // recover scores
            courses_[course_id].ChangeScore(info.id,
                                            score_before);
        }

        students_.erase(iter);
        students_.emplace(info.id, new_student);
    }
    else  // id stay the same
    {
        iter->second.set_info(info);
    }

    return true;
}

bool Manager::AddCourse(const CourseInfo &info)
{
    if (courses_.count(info.id) != 0)  // id has been occupied
        return false;

    courses_.emplace(info.id, Course(info));
    return true;
}

bool Manager::RemoveCourse(Course::IDType course_id)
{
    auto iter = courses_.find(course_id);
    if (iter == courses_.end())  // course not found
        return false;

    auto final_score = iter->second.final_score();
    for (ScorePiece score_piece : final_score)
    {
        students_[score_piece.id].RemoveCourse(course_id);
    }
    courses_.erase(iter);
    return true;
}

bool Manager::HasCourse(Course::IDType course_id) const
{
    return courses_.count(course_id) != 0;
}

Manager::CourseIterator Manager::FindCourse(Course::IDType course_id) const
{
    return CourseIterator(courses_.find(course_id));
}

bool Manager::SetCourseInfo(Course::IDType course_id,
                            const CourseInfo &info)
{
    auto iter = courses_.find(course_id);
    if (iter == courses_.end())
        return false;

    if (course_id != info.id)  // the id has changed
    {
        if (courses_.count(info.id) != 0)
            return false;  // new id has been taken

        Course new_course(iter->second);
        new_course.set_info(info);
        // updating IDs
        for (ScorePiece score_piece : new_course.final_score())
        {
            students_[score_piece.id].RemoveCourse(course_id);
            students_[score_piece.id].AddCourse(info.id);
        }

        courses_.erase(iter);
        courses_.emplace(info.id, new_course);
    }
    else  // the id stay the same
    {
        iter->second.set_info(info);
    }

    return true;
}

bool Manager::AddStudentToCourse(Student::IDType student_id,
                                 Course::IDType course_id)
{
    auto iter_student = students_.find(student_id);
    auto iter_course = courses_.find(course_id);

    if (iter_student == students_.end() ||
        iter_course == courses_.end())
        return false;

    return iter_course->second.AddStudent(iter_student->second);
}

bool Manager::AddStudentToCourse(
        const std::vector<Student::IDType> &student_ids,
        Course::IDType course_id)
{
    auto iter_course = courses_.find(course_id);

    if (iter_course == courses_.end())
        return false;

    bool added_at_least_one = false;
    for (auto student_id : student_ids)
    {
        auto iter_student = students_.find(student_id);
        if (iter_student != students_.end())
        {
            if (iter_course->second.AddStudent(iter_student->second))
                added_at_least_one = true;
        }
    }

    return added_at_least_one;
}

bool Manager::RemoveStudentFromCourse(Student::IDType student_id,
                                      Course::IDType course_id)
{
    auto iter_student = students_.find(student_id);
    auto iter_course = courses_.find(course_id);

    if (iter_student == students_.end() ||
        iter_course == courses_.end())
        return false;

    iter_course->second.RemoveStudent(iter_student->second);
    return true;
}

bool Manager::RecordFinalScore(const Course::IDType &course_id,
                               const FinalScore &final_score,
                               std::vector<Student::IDType> &unscored_students)
{
    auto iter = courses_.find(course_id);
    if (iter == courses_.end())
        return false;

    iter->second.RecordFinalScore(final_score, unscored_students);
    return true;
}

void Manager::RemoveFinalScore(const Course::IDType &course_id)
{
    auto iter = courses_.find(course_id);

    if (iter != courses_.end())
        iter->second.RemoveFinalScore();
}

ScoreType Manager::GetScore(Student::IDType student_id,
                            const Course::IDType &course_id) const
{
    auto iter = courses_.find(course_id);
    if (iter == courses_.end())
        return false;

    return iter->second.GetScore(student_id);
}

bool Manager::ChangeScore(Student::IDType student_id,
                          const Course::IDType &course_id,
                          ScoreType new_score)
{
    auto iter = courses_.find(course_id);
    if (iter == courses_.end())
        return false;

    return iter->second.ChangeScore(student_id, new_score);
}

}  // namespace SAM

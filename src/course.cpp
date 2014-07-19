#include <algorithm>
#include <iomanip>
#include <iterator>
#include <sstream>

#include "course.h"

namespace SAM {

Course::Course(const CourseInfo &info)
        : info_(info),
          final_score_()
{
}

void Course::RecordFinalScore(const FinalScore &final_score,
                              std::vector<Student::IDType> &unscored_students)
{
    for (const auto &score_piece : final_score)
    {
        ChangeScore(score_piece.id, score_piece.score);
    }

    for (const auto &score_piece : final_score_)
    {
        if (score_piece.score == kInvalidScore)
        {
            // no score given
            unscored_students.push_back(score_piece.id);
        }
    }
}

void Course::RemoveFinalScore()
{
    for (auto &score_piece : final_score_)
        score_piece.score = kInvalidScore;
}

bool Course::AddStudent(Student &student)
{
    if (IsFull())
        return false;

    student.AddCourse(info_.id);

    auto student_id = student.info().id;
    auto range_pair = EqualRange(student_id);
    if (range_pair.first == range_pair.second)  // have not record this student
    {
        final_score_.insert(range_pair.first,
                            ScorePiece{student_id, kInvalidScore});
    }

    return true;
}

void Course::RemoveStudent(Student &student)
{
    student.RemoveCourse(info_.id);

    auto range_pair = EqualRange(student.info().id);
    final_score_.erase(range_pair.first, range_pair.second);
}

bool Course::HasStudent(Student::IDType student_id) const
{
    auto range_pair = EqualRange(student_id);
    return range_pair.first != range_pair.second;
}

ScoreType Course::GetScore(Student::IDType student_id) const
{
    auto range_pair = EqualRange(student_id);

    if (range_pair.first == range_pair.second)
        return kInvalidScore;  // invalid student_id
    else
        return range_pair.first->score;
}

bool Course::ChangeScore(Student::IDType student_id, ScoreType new_score)
{
    auto range_pair = EqualRange(student_id);

    if (range_pair.first != range_pair.second)
    {
        range_pair.first->score = new_score;
        return true;
    }

    return false;
}

std::string Course::Heading()
{
    using std::setw;

    static std::string heading;
    static bool heading_is_valid = false;

    if (!heading_is_valid)
    {
        std::ostringstream oss;
        oss << setw(id_width) << "ID" << ' '
            << setw(name_width + 3) << "课程名" << ' '
            << setw(department_width + 2) << "院系" << ' '
            << setw(credit_width + 2) << "学分" << ' '
            << setw(capacity_width + 2) << "人数" << ' '
            << setw(teacher_name_width + 2) << "教师";
        heading = oss.str();
        heading_is_valid = true;
    }

    return heading;
}

std::ostream & operator<<(std::ostream &os, const Course &course)
{
    using std::setw;
    const CourseInfo &info = course.info();

    os << setw(Course::id_width + 1) << info.id << ' ';  // 1 Chinese char
    PrintChinese(os, info.name, Course::name_width) << ' ';
    PrintChinese(os, kDepartmentName[info.department],
                 Course::department_width) << ' ';
    os << setw(Course::credit_width) << info.credit << ' '
       << setw(Course::capacity_width)
       << (std::to_string(course.StudentNumber()) + '/' +
           std::to_string(info.capacity)) << ' ';
    PrintChinese(os, info.teacher_name, Course::teacher_name_width);

    return os;
}


}  // namespace SAM

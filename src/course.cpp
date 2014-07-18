#include <algorithm>
#include <iterator>
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

}  // namespace SAM

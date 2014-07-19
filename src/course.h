#ifndef SAM_COURSE_H_
#define SAM_COURSE_H_

#include <algorithm>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "common.h"
#include "student.h"

namespace SAM {

// Store the basic information of a course, and manage final score.
// id shall be UNIQUE, so a same class in different semester should have
// different id.
class Course
{
 public:
    typedef CourseInfo::IDType IDType;

    Course() = default;
    explicit Course(const CourseInfo &info);

    // The course-taking information will be updated after calling these.
    // Cannot add if the course if full
    bool AddStudent(Student &student);
    void RemoveStudent(Student &student);
    // Check whether a certain student is in the course.
    bool HasStudent(Student::IDType student_id) const;
    bool HasStudent(const Student &student) const
    { return HasStudent(student.info().id); }

    // If a student is unscored after update, his ID will be added to the back
    // of unscored_students.
    // If a student has more than one score, the last one will be taken
    void RecordFinalScore(const FinalScore &final_score,
                          std::vector<Student::IDType> &unscored_students);
    void RemoveFinalScore();

    // If the student is not in this course, kInvalidScore will be returned.
    ScoreType GetScore(Student::IDType student_id) const;
    ScoreType GetScore(const Student &student) const
    { return GetScore(student.info().id); }

    // If the arguments are invalid, nothing will be done
    bool ChangeScore(Student::IDType student_id, ScoreType new_score);

    bool IsFull() const { return final_score_.size() == info_.capacity; }

    // accessors
    const CourseInfo & info() const { return info_; }
    const FinalScore & final_score() const { return final_score_; }
    // to get a student list, use final_score()

    // mutators
    void set_info(const CourseInfo &info) { info_ = info; }

    // Return the heading for display
    static std::string Heading();

 private:
    std::pair<FinalScore::const_iterator, FinalScore::const_iterator>
    EqualRange(Student::IDType student_id) const
    {
        return std::equal_range(final_score_.begin(),
                                final_score_.end(),
                                ScorePiece{student_id, 0},
                                [](const ScorePiece &lhs,
                                   const ScorePiece &rhs)
                                { return lhs.id < rhs.id; });
    }

    std::pair<FinalScore::iterator, FinalScore::iterator>
    EqualRange(Student::IDType student_id)
    {
        return std::equal_range(final_score_.begin(),
                                final_score_.end(),
                                ScorePiece{student_id, 0},
                                [](const ScorePiece &lhs,
                                   const ScorePiece &rhs)
                                { return lhs.id < rhs.id; });
}

    CourseInfo info_;

    FinalScore final_score_;  // always sorted
};

std::ostream & operator<<(std::ostream &os, const Course &course);


}  // namespace SAM

#endif  // SAM_COURSE_H_

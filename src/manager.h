#ifndef SAM_MANAGER_H_
#define SAM_MANAGER_H_

#include <iterator>
#include <map>
#include <string>
#include <vector>

#include "student.h"
#include "course.h"

namespace SAM {

// Manage students and courses.
// Every student/course should has an unique id.
class Manager
{
 public:
    template <typename KeyType, typename ItemType>
    class MapItemIterator
            : public std::iterator<std::bidirectional_iterator_tag, ItemType>
    {
     private:
        typedef typename std::map<KeyType, ItemType>::const_iterator MapIter;

     public:
        explicit MapItemIterator(MapIter map_iter) : map_iter_(map_iter) {}

        bool operator==(const MapItemIterator &rhs) const
        { return map_iter_ == rhs.map_iter_; }

        bool operator!=(const MapItemIterator &rhs) const
        { return map_iter_ != rhs.map_iter_; }

        const ItemType & operator*() const { return map_iter_->second; }
        const ItemType * operator->() const { return &(map_iter_->second); }

        MapItemIterator & operator++()
        {
            ++map_iter_;
            return *this;
        }

        MapItemIterator operator++(int)
        {
            return MapItemIterator(map_iter_++);
        }

        MapItemIterator & operator--()
        {
            --map_iter_;
            return *this;
        }

        MapItemIterator operator--(int)
        {
            return MapItemIterator(map_iter_--);
        }

     private:
        MapIter map_iter_;
    };

    typedef MapItemIterator<Student::IDType, Student> StudentIterator;
    typedef MapItemIterator<Course::IDType, Course> CourseIterator;

    Manager();

    // ======================= Operations for students =======================
    bool AddStudent(const StudentInfo &student_info);
    bool RemoveStudent(Student::IDType student_id);
    bool HasStudent(Student::IDType student_id) const;
    StudentIterator FindStudent(Student::IDType student_id) const;

    // IDs that courses have will be updated if needed.
    // If the new ID has been taken, nothing will be changed.
    bool SetStudentInfo(Student::IDType student_id,
                        const StudentInfo &info);


    // ======================= Operations for courses =======================
    bool AddCourse(const CourseInfo &info);
    bool RemoveCourse(Course::IDType course_id);
    bool HasCourse(Course::IDType course_id) const;
    CourseIterator FindCourse(Course::IDType course_id) const;

    // IDs that courses have will be updated if needed.
    // If the new ID has been taken, nothing will be changed.
    bool SetCourseInfo(Course::IDType course_id,
                       const CourseInfo &info);


    // ================== Operations for students & courses ==================
    bool AddStudentToCourse(Student::IDType student_id,
                            Course::IDType course_id);
    bool AddStudentToCourse(const std::vector<Student::IDType> &student_ids,
                            Course::IDType course_id);
    bool RemoveStudentFromCourse(Student::IDType student_id,
                                 Course::IDType course_id);

    // ===================== Operations for final score =====================
    // Record final scores.
    // If a student is unscored after update, his ID will be added to the back
    // of unscored_students.
    // If a student has more than one score, the last one will be taken
    bool RecordFinalScore(const Course::IDType &course_id,
                          const FinalScore &final_score,
                          std::vector<Student::IDType> &unscored_students);

    // Set all the scores to kInvalidScore
    void RemoveFinalScore(const Course::IDType &course_id);

    // If the arguments are invalid, kInvalidScore will be returned.
    ScoreType GetScore(Student::IDType student_id,
                       const Course::IDType &course_id) const;

    bool ChangeScore(Student::IDType student_id,
                     const Course::IDType &course_id,
                     ScoreType new_score);

    // accessors
    StudentIterator student_begin() const
    { return StudentIterator(students_.cbegin()); }

    StudentIterator student_end() const
    { return StudentIterator(students_.cend()); }

    CourseIterator course_begin() const
    { return CourseIterator(courses_.cbegin()); }

    CourseIterator course_end() const
    { return CourseIterator(courses_.cend()); }

 private:
    std::map<Student::IDType, Student> students_;
    std::map<Course::IDType, Course> courses_;
};

}  // namespace SAM

#endif  // SAM_MANAGER_H_

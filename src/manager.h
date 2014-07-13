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
// Every student/course should has a unique id.
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

        bool operator==(const MapItemIterator &rhs)
        { return map_iter_ == rhs.map_iter_; }

        bool operator!=(const MapItemIterator &rhs)
        { return map_iter_ != rhs.map_iter_; }

        const ItemType & operator*() { return map_iter_->second; }
        const ItemType * operator->() { return &(map_iter_->second); }

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
    bool SearchStudent(Student::IDType student_id,
                       Student &student_found) const;

    // IDs that courses have will be updated if needed.
    // If the new ID has been taken, nothing will be changed.
    bool SetStudentInfo(Student::IDType student_id,
                        const StudentInfo &info);


    // ======================= Operations for courses =======================
    bool AddCourse(const CourseInfo &info);
    bool RemoveCourse(Course::IDType course_id);
    bool HasCourse(Course::IDType course_id) const;
    bool SearchCourse(Course::IDType course_id,
                      Course &course_found) const;

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

    // accessors
    StudentIterator student_begin() const { return students_.cbegin(); }
    StudentIterator student_end() const { return students_.cend(); }

    CourseIterator course_begin() const { return courses_.cbegin(); }
    CourseIterator course_end() const { return courses_.cend(); }

 private:
    std::map<Student::IDType, Student> students_;
    std::map<Course::IDType, Course> courses_;
};

}  // namespace SAM

#endif  // SAM_MANAGER_H_

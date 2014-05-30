#ifndef SAM_EXAM_H_
#define SAM_EXAM_H_

#include <string>
#include "date.h"
#include "course.h"
#include "student.h"

namespace SAM {

struct ExamInfo
{
    std::string name;
    Date date;
};

struct Exam
{
    typedef double ScoreType;

    Exam(Course::IDType course_id, std::string name, Date date)
            : course_id(course_id),
              info{name, date},
              result() {}

    Course::IDType course_id;
    ExamInfo info;

    std::vector<std::pair<Student::IDType, ScoreType>> result;
};

}  // namespace SAM

#endif  // SAM_EXAM_H_

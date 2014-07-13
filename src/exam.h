#ifndef SAM_EXAM_H_
#define SAM_EXAM_H_

#include <string>
#include <vector>

#include "common.h"

namespace SAM {

struct Exam
{
    typedef ExamInfo::ScoreType ScoreType;

    Exam(CourseInfo::IDType course_id, std::string name, Date date)
            : info{course_id, name, date},
              result() {}

    ExamInfo info;

    std::vector<std::pair<StudentInfo::IDType, ScoreType>> result;
};

}  // namespace SAM

#endif  // SAM_EXAM_H_

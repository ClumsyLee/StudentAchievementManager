#ifndef SAM_EXAM_H_
#define SAM_EXAM_H_

#include <string>
#include <vector>

#include "date.h"
#include "common.h"

namespace SAM {

struct Exam
{
    typedef double ScoreType;

    Exam(BasicCourseInfo::IDType course_id, std::string name, Date date)
            : course_id(course_id),
              info{name, date},
              result() {}

    BasicCourseInfo::IDType course_id;
    BasicExamInfo info;

    std::vector<std::pair<BasicStudentInfo::IDType, ScoreType>> result;
};

}  // namespace SAM

#endif  // SAM_EXAM_H_

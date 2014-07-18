// This analyser is designed to used in THU

#ifndef SAM_ANALYSER_H_
#define SAM_ANALYSER_H_

#include <functional>
#include <vector>
#include "common.h"
#include "manager.h"

namespace SAM {

struct CourseIDInfo
{
    CourseIDInfo() = default;
    explicit CourseIDInfo(const CourseInfo::IDType &course_id);
    explicit operator CourseInfo::IDType() const;

    enum Season { SPRING, SUMMER, FALL };
    static const char * kSeasonStr[3];

    int id;
    int year;
    Season season;
};

struct TranscriptEntry
{
    CourseInfo course_info;

    ScoreType score;
    ScoreType min_score;
    ScoreType max_score;

    int student_num;
    int rank;
};

struct Transcript
{
    StudentInfo student_info;
    std::vector<TranscriptEntry> final_scores;
    ScoreType gpa;
};


class Analyser
{
 public:
    Analyser();

    bool GenerateTranscript(const Manager &manager,
                            StudentInfo::IDType student_id,
                            Transcript &transcript);

    bool GenerateTranscript(const Manager &manager,
                            StudentInfo::IDType student_id,
                            std::function<bool(const Course &)> course_filter,
                            Transcript &transcript);

 private:

};

}  // namespace SAM

#endif  // SAM_ANALYSER_H_

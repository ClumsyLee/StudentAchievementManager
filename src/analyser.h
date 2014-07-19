// This analyser is designed to used in THU

#ifndef SAM_ANALYSER_H_
#define SAM_ANALYSER_H_

#include <functional>
#include <ostream>
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
    int total_credit;
    ScoreType gpa;
};

// Print the transcript to os
std::ostream & operator<<(std::ostream &os, const Transcript &transcript);


class Analyser
{
 public:
    typedef std::function<bool(const Course &)> CourseFilter;

    bool GenerateTranscript(const Manager &manager,
                            StudentInfo::IDType student_id,
                            Transcript &transcript)
    {
        return GenerateTranscript(manager,
                                  student_id,
                                  [](const Course &) { return true; },
                                  transcript);
    }

    bool GenerateTranscript(const Manager &manager,
                            StudentInfo::IDType student_id,
                            CourseFilter course_filter,
                            Transcript &transcript);

 private:
    void SetMaxMinRank(const Manager::CourseIterator &crs_iter,
                       TranscriptEntry &entry);
};

}  // namespace SAM

#endif  // SAM_ANALYSER_H_

// Designed to used in THU
#include <limits>
#include "analyser.h"

namespace SAM {

const char * CourseIDInfo::kSeasonStr[3] = {
    "春", "夏", "秋"
};

// Assume IDType is std::string
// and the format is yyyyS-xxxxxxxxx
// where yyyy is the year, S is the season.
CourseIDInfo::CourseIDInfo(const CourseInfo::IDType &course_id)
{
    std::size_t dash_position = course_id.find('-');

    std::size_t year_end;
    year = std::stoi(course_id, &year_end);
    std::string season_str =
            course_id.substr(year_end, dash_position - year_end);

    if (season_str == kSeasonStr[0])
        season = SPRING;
    else if (season_str == kSeasonStr[1])
        season = SUMMER;
    else
        season = FALL;
    // no winter

    id = std::stoi(course_id.substr(dash_position + 1));
}

CourseIDInfo::operator CourseInfo::IDType() const
{
    CourseInfo::IDType course_id = std::to_string(year);
    course_id += kSeasonStr[season];
    course_id += '-';
    course_id += std::to_string(id);

    return course_id;
}

std::ostream & operator<<(std::ostream &os, const Transcript &transcript)
{
    os << "姓名：" << transcript.student_info.name;
}

bool Analyser::GenerateTranscript(const Manager &manager,
                                  StudentInfo::IDType student_id,
                                  CourseFilter course_filter,
                                  Transcript &transcript)
{
    auto stu_iter = manager.FindStudent(student_id);
    if (stu_iter == manager.student_end())
        return false;

    transcript.student_info = stu_iter->info();
    ScoreType weighted_sum = 0;
    transcript.total_credit = 0;

    for (Course::IDType course_id : stu_iter->courses_taken())
    {
        // building a transcript entry
        TranscriptEntry entry;

        auto crs_iter = manager.FindCourse(course_id);
        if (crs_iter == manager.course_end())
        {
            // internal failure in manager
            std::exit(EXIT_FAILURE);
        }

        if (!course_filter(*crs_iter))
            continue;

        entry.course_info = crs_iter->info();
        entry.score = crs_iter->GetScore(student_id);
        entry.student_num = crs_iter->StudentNumber();
        SetMaxMinRank(crs_iter, entry);

        transcript.final_scores.push_back(entry);

        // for credit and GPA
        if (entry.score != kInvalidScore)
        {
            int credit = entry.course_info.credit;

            weighted_sum += entry.score * credit;
            transcript.total_credit += credit;
        }
    }

    if (transcript.total_credit != 0)
        transcript.gpa = weighted_sum / transcript.total_credit;
    else
        transcript.gpa = kInvalidScore;

    return true;
}

void Analyser::SetMaxMinRank(const Manager::CourseIterator &crs_iter,
                             TranscriptEntry &entry)
{
    ScoreType min = std::numeric_limits<ScoreType>::max();
    ScoreType max = std::numeric_limits<ScoreType>::lowest();
    int rank_now = 1;
    bool exist_valid_score = false;

    for (const ScorePiece &score_piece : crs_iter->final_score())
    {
        ScoreType score = score_piece.score;

        if (score == kInvalidScore)  // ignore invalid scores
            continue;

        exist_valid_score = true;

        if (score < min)
            min = score;
        if (score > max)
            max = score;

        if (score > entry.score)
            rank_now++;
    }

    if (!exist_valid_score)
        max = min = kInvalidScore;

    entry.max_score = max;
    entry.min_score = min;
    entry.rank = (entry.score == kInvalidScore ? 0 : rank_now);
}


}  // namespace SAM

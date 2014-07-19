// Designed to used in THU
#include <limits>
#include <iomanip>
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
    using std::endl;
    const StudentInfo &info = transcript.student_info;

    os <<
"                               清华大学学生成绩单                               \n"
"\n"
"姓名：" << info.name << "\n"
"学号：" << info.id << "\n"
"性别：" << (info.is_male ? "男": "女") << "\n"
"院系：" << kDepartmentName[info.department] << "\n"
"\n"
" 课程号    学期               课程名              学分  最低/成绩/最高  排名  \n"
"================================================================================\n";

    for (const TranscriptEntry &entry : transcript.final_scores)
    {
        using std::setw;
        using std::to_string;

        os << std::left;

        CourseIDInfo course_id_info(entry.course_info.id);
        os << setw(8) << course_id_info.id << "  "
           << setw(4) << course_id_info.year
           << CourseIDInfo::kSeasonStr[course_id_info.season] << "  ";

        PrintChinese(os, entry.course_info.name, 30) << "  "

           << setw(4) << entry.course_info.credit << "  ";

        os.width(4);
        PrintScore(os, entry.min_score) << '/';
        os.width(4);
        PrintScore(os, entry.score) << '/';
        os.width(4);
        PrintScore(os, entry.max_score) << "  ";

        os << ((entry.rank != 0 ? to_string(entry.rank) : "*")+
               '/' +
               to_string(entry.student_num))
           << endl;

        os << std::right;
    }

    os <<
"\n"
"总学分: " << transcript.total_credit << "\n"
"GPA: " << transcript.gpa << endl;

    return os;
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

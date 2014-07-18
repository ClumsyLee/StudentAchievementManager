// Designed to used in THU
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

}  // namespace SAM

#include <fstream>
#include <sstream>
#include "io.h"

namespace SAM {

bool ManagerReader::Read(const std::string &student_file_name,
                         const std::string &course_file_name,
                         Manager &manager)
{
    std::ifstream student_fin(student_file_name);
    std::ifstream course_fin(course_file_name);

    if (!student_fin.is_open() || !course_fin.is_open())
        return false;

    // read students
    std::string line;
    while (std::getline(student_fin, line))
    {
        StudentInfo info;
        MakeStudentInfo(line, info);
        manager.AddStudent(info);
    }
    student_fin.close();

    // read courses
    while (std::getline(course_fin, line))
    {
        CourseInfo course_info;
        MakeCourseInfo(line, course_info);
        manager.AddCourse(course_info);

        // recover student info
        if (!std::getline(course_fin, line))
            return false;

        std::istringstream iss(line);
        Student::IDType student_id;
        ScoreType score;

        while (iss >> student_id >> score)
        {
            manager.AddStudentToCourse(student_id, course_info.id);
            manager.ChangeScore(student_id, course_info.id, score);
        }
    }

    return true;
}

bool ManagerReader::ReadFinalScore(
        const std::string &file_name,
        Manager &manager,
        CourseInfo::IDType course_id,
        std::vector<Student::IDType> &unscored_students)
{
    if (!manager.HasCourse(course_id))
        return false;

    std::ifstream fin(file_name);
    if (!fin.is_open())
        return false;

    ScorePiece score_piece;
    FinalScore final_score;

    while (fin >> score_piece.id >> score_piece.score)
    {
        final_score.push_back(score_piece);
    }

    manager.RecordFinalScore(course_id, final_score, unscored_students);

    return true;
}

bool ManagerWriter::Write(const std::string &student_file_name,
                          const std::string &course_file_name,
                          const Manager &manager)
{
    std::ofstream student_fout(student_file_name);
    std::ofstream course_fout(course_file_name);

    if (!student_fout.is_open() || !course_fout.is_open())
        return false;

    // write students
    for (auto iter = manager.student_begin();
         iter != manager.student_end();
         ++iter)
    {
        student_fout << to_string(iter->info()) << std::endl;
    }
    student_fout.close();

    // write courses
    for (auto iter = manager.course_begin();
         iter != manager.course_end();
         ++iter)
    {
        course_fout << to_string(iter->info()) << std::endl;

        for (const ScorePiece &score_piece : iter->final_score())
        {
            course_fout << score_piece.id << ' ' << score_piece.score << ' ';
        }
        course_fout << std::endl;
    }

    return true;
}


}  // namespace SAM

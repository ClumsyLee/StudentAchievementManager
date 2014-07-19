#include <iomanip>
#include <iostream>
#include <sstream>

#include "command_line_interface.h"
#include "io.h"

namespace SAM {

static const int kScoreWidth = 5;

std::vector<CommandLineInterface::Command> CommandLineInterface::commands_ = {
    {"ls-stu", &CommandLineInterface::ListStudents},
    {"add-stu", &CommandLineInterface::AddStudent},
    {"rm-stu", &CommandLineInterface::RemoveStudent},
    {"stu", &CommandLineInterface::ShowStudent},

    {"ls-crs", &CommandLineInterface::ListCourses},
    {"add-crs", &CommandLineInterface::AddCourse},
    {"rm-crs", &CommandLineInterface::RemoveCourse},
    {"crs", &CommandLineInterface::ShowCourse},

    {"reg", &CommandLineInterface::RegisterToCourse},
    {"drop", &CommandLineInterface::DropFromCourse},

    {"save", &CommandLineInterface::Save},
    {"load", &CommandLineInterface::Load}
};


CommandLineInterface::CommandLineInterface()
        : prompt_("SAM-0.3: "),
          command_stream_(),
          manager_()
{
}

int CommandLineInterface::Run(int argc, const char* const argv[])
{
    std::string command;

    std::cout << prompt_;
    while (std::getline(std::cin, command))
    {
        command_stream_.clear();
        command_stream_.str(command);  // add this line to stream

        if (ParseAndRunCommand())
            return 0;
        std::cout << prompt_;
    }

    return 0;
}


bool CommandLineInterface::ParseAndRunCommand()
{
    std::string command;
    if (!(command_stream_ >> command))  // if fail, command will stay empty
        return false;

    if (command == "quit")
        return true;

    for (const Command &legal_command : commands_)
    {
        if (legal_command.first == command)
        {
            std::cout << std::endl;
            legal_command.second(*this);
            std::cout << std::endl;
            return false;
        }
    }

    std::cout << command << ": command not found\n";
    return false;
}


void CommandLineInterface::ListStudents()
{
    std::cout << Student::Heading() << std::endl
              << std::string(Student::HeadingSize(), '-') << std::endl;

    for (auto iter = manager_.student_begin(); iter != manager_.student_end();
         ++iter)
    {
        std::cout << *iter << std::endl;
    }
}

void CommandLineInterface::AddStudent()
{
    StudentInfo info;
    if (!MakeStudentInfo(command_stream_.str(), info))
    {
        std::cout << "Fail to construct student info from \""
                  << command_stream_.str() << "\"\n";
    }
    else if (!manager_.AddStudent(info))
    {
        std::cout << "Fail to add student: ID " << info.id
                  << " has been occupied\n";
    }
}

void CommandLineInterface::RemoveStudent()
{
    StudentInfo::IDType id;
    if (command_stream_ >> id)
    {
        if (!manager_.RemoveStudent(id))
            std::cout << "Fail to remove student: no student with ID "
                      << id << std::endl;
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: rm-stu <ID>\n";
    }
}

void CommandLineInterface::ShowStudent()
{
    using std::cout;

    StudentInfo::IDType id;
    if (command_stream_ >> id)
    {
        auto stu_iter = manager_.FindStudent(id);
        if (stu_iter != manager_.student_end())
        {
            cout << Student::Heading() << std::endl
                 << std::string(Student::HeadingSize(), '-') << std::endl
                 << *stu_iter << std::endl
                 << "\n==================== 所选课程 ====================\n\n"
                 << Course::Heading() << ' '
                 << std::setw(kScoreWidth + 2) << "分数" << std::endl
                 << std::string(Course::HeadingSize() + 1 + kScoreWidth, '-')
                 << std::endl;

            for (const Course::IDType &course_id : stu_iter->courses_taken())
            {
                auto crs_iter = manager_.FindCourse(course_id);
                if (crs_iter == manager_.course_end())
                {
                    cout << "ERROR: Failed to find a course that should "
                                 "exist\n"
                                 "Student ID: " << id << "\n"
                                 "Course fail: " << course_id << "\n"
                                 "Students now:\n";
                    ListStudents();
                    cout << "Courses now:\n";
                    ListCourses();

                    std::exit(EXIT_FAILURE);
                }

                cout << *crs_iter << ' ';

                cout.width(kScoreWidth);
                ScoreType score = crs_iter->GetScore(id);
                if (score == kInvalidScore)
                    cout << "*";
                else
                    cout << score;

                cout << std::endl;
            }
        }
        else
        {
            std::cout << "No student with ID " << id << std::endl;
        }
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: stu <ID>\n";
    }
}

void CommandLineInterface::ListCourses()
{
    std::cout << Course::Heading() << std::endl
              << std::string(Course::HeadingSize(), '-') << std::endl;

    for (auto iter = manager_.course_begin(); iter != manager_.course_end();
         ++iter)
    {
        std::cout << *iter << std::endl;
    }
}

void CommandLineInterface::AddCourse()
{
    CourseInfo info;
    if (!MakeCourseInfo(command_stream_.str(), info))
    {
        std::cout << "Fail to construct course info from \""
                  << command_stream_.str() << "\"\n";
    }
    else if (!manager_.AddCourse(info))
    {
        std::cout << "Fail to add course: ID " << info.id
                  << " has been occupied\n";
    }
}

void CommandLineInterface::RemoveCourse()
{
    CourseInfo::IDType id;
    if (command_stream_ >> id)
    {
        if (!manager_.RemoveCourse(id))
            std::cout << "Fail to remove course: no course with ID "
                      << id << std::endl;
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: rm-crs <ID>\n";
    }
}

void CommandLineInterface::ShowCourse()
{
    using std::cout;

    CourseInfo::IDType id;
    if (command_stream_ >> id)
    {
        auto crs_iter = manager_.FindCourse(id);
        if (crs_iter != manager_.course_end())
        {
            cout << Course::Heading() << std::endl
                 << std::string(Course::HeadingSize(), '-') << std::endl
                 << *crs_iter << std::endl
                 << "\n==================== 课内学生 ====================\n\n"
                 << Student::Heading() << ' '
                 << std::setw(kScoreWidth + 2) << "分数" << std::endl
                 << std::string(Student::HeadingSize() + 1 + kScoreWidth, '-')
                 << std::endl;

            for (const ScorePiece &score_piece : crs_iter->final_score())
            {
                auto stu_iter = manager_.FindStudent(score_piece.id);
                if (stu_iter == manager_.student_end())
                {
                    cout << "ERROR: Failed to find a student that should "
                                 "exist\n"
                                 "Course ID: " << id << "\n"
                                 "Student fail: " << score_piece.id << "\n"
                                 "Students now:\n";
                    ListStudents();
                    cout << "Courses now:\n";
                    ListCourses();

                    std::exit(EXIT_FAILURE);
                }
                cout << *stu_iter << ' ';

                cout.width(kScoreWidth);
                if (score_piece.score == kInvalidScore)
                    cout << '*';
                else
                    cout << score_piece.score;

                cout << std::endl;
            }
        }
        else
        {
            cout << "No course with ID " << id << std::endl;
        }
    }
    else
    {
        cout << "Syntax error\n"
             << "Usage: crs <ID>\n";
    }
}

void CommandLineInterface::RegisterToCourse()
{
    Student::IDType student_id;
    Course::IDType course_id;

    if (command_stream_ >> student_id >> course_id)
    {
        if (!manager_.AddStudentToCourse(student_id, course_id))
        {
            std::cout << "Fail to register student " << student_id << " to "
                         "course " << course_id << "\n"
                         "Check the student and the course for more info\n";
        }
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: register <student ID> <course ID>\n";
    }
}

void CommandLineInterface::DropFromCourse()
{
    Student::IDType student_id;
    Course::IDType course_id;

    if (command_stream_ >> student_id >> course_id)
    {
        if (!manager_.RemoveStudentFromCourse(student_id, course_id))
        {
            std::cout << "Fail to drop student " << student_id << " from "
                         "course " << course_id << "\n"
                         "Check the student and the course for more info\n";
        }
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: drop <student ID> <course ID>\n";
    }
}

void CommandLineInterface::Save()
{
    ManagerWriter writer;

    if (writer.Write("students.dat", "courses.dat", manager_))
        std::cout << "Saved\n";
    else
        std::cout << "Failed to save\n";
}


void CommandLineInterface::Load()
{
    ManagerReader reader;

    if (reader.Read("students.dat", "courses.dat", manager_))
        std::cout << "Loaded\n";
    else
        std::cout << "Failed to load\n";
}

}  // namespace SAM

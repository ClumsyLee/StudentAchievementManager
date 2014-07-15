#include <iostream>
#include <sstream>

#include "command_line_interface.h"
#include "io.h"

namespace SAM {

CommandLineInterface::CommandLineInterface()
        : commands_{
              {"ls-stu", &CommandLineInterface::ListStudents},
              {"add-stu", &CommandLineInterface::AddStudent},
              {"rm-stu", &CommandLineInterface::RemoveStudent},
              {"stu", &CommandLineInterface::ShowStudent},

              {"ls-crs", &CommandLineInterface::ListCourses},
              {"add-crs", &CommandLineInterface::AddCourse},
              {"rm-crs", &CommandLineInterface::RemoveCourse},
              {"crs", &CommandLineInterface::ShowCourse},

              {"register", &CommandLineInterface::RegisterToCourse},
              {"drop", &CommandLineInterface::DropFromCourse},

              {"save", &CommandLineInterface::Save},
              {"load", &CommandLineInterface::Load}
          },
          prompt_("SAM-0.2: "),
          manager_()
{
}

int CommandLineInterface::Run(int argc, const char* const argv[])
{
    std::string command;

    std::cout << prompt_;
    while (std::getline(std::cin, command))
    {
        if (ParseAndRunCommand(command))
            return 0;
        std::cout << prompt_;
    }

    std::cout << std::endl;

    return 0;
}


bool CommandLineInterface::ParseAndRunCommand(const std::string &command)
{
    for (const Command &legal_command : commands_)
    {
        const std::string &command_name = legal_command.first;
        auto command_size = command_name.size();

        if (command.compare(0, command_size, command_name) == 0)
        {
            legal_command.second(*this, command.substr(command_size));
            return false;
        }
    }

    std::cout << command << ": command not found\n";
    return false;
}


void CommandLineInterface::ListStudents(const std::string &args)
{
    for (auto iter = manager_.student_begin(); iter != manager_.student_end();
         ++iter)
    {
        std::cout << iter->info().ToString() << std::endl;
    }
}

void CommandLineInterface::AddStudent(const std::string &args)
{
    StudentInfo info(args);
    if (!manager_.AddStudent(info))
        std::cout << "Fail to add student: ID " << info.id
                  << " has been occupied\n";
}

void CommandLineInterface::RemoveStudent(const std::string &args)
{
    std::istringstream iss(args);

    StudentInfo::IDType id;
    if (iss >> id)
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

void CommandLineInterface::ShowStudent(const std::string &args)
{
    std::istringstream iss(args);

    StudentInfo::IDType id;
    if (iss >> id)
    {
        auto stu_iter = manager_.FindStudent(id);
        if (stu_iter != manager_.student_end())
        {
            std::cout << stu_iter->info().ToString() << std::endl;
            std::cout << "Course taken:\n";

            for (const Course::IDType &course_id : stu_iter->courses_taken())
            {
                auto crs_iter = manager_.FindCourse(course_id);

                if (crs_iter == manager_.course_end())
                {
                    std::cout << "ERROR: Failed to find a course that should "
                                 "exist\n"
                                 "Student ID: " << id << "\n"
                                 "Course fail: " << course_id << "\n"
                                 "Students now:\n";
                    ListStudents(args);
                    std::cout << "Courses now:\n";
                    ListCourses(args);

                    std::exit(EXIT_FAILURE);
                }

                std::cout << crs_iter->info().ToString() << std::endl;
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

void CommandLineInterface::ListCourses(const std::string &args)
{
    for (auto iter = manager_.course_begin(); iter != manager_.course_end();
         ++iter)
    {
        std::cout << iter->info().ToString() << std::endl;
    }
}

void CommandLineInterface::AddCourse(const std::string &args)
{
    CourseInfo info(args);

    if (!manager_.AddCourse(info))
        std::cout << "Fail to add course: ID " << info.id
                  << " has been occupied\n";
}

void CommandLineInterface::RemoveCourse(const std::string &args)
{
    std::istringstream iss(args);

    CourseInfo::IDType id;
    if (iss >> id)
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

void CommandLineInterface::ShowCourse(const std::string &args)
{
    std::istringstream iss(args);

    CourseInfo::IDType id;
    if (iss >> id)
    {
        auto crs_iter = manager_.FindCourse(id);
        if (crs_iter != manager_.course_end())
        {
            std::cout << crs_iter->info().ToString() << std::endl;
            std::cout << "Student(s) in this course:\n";

            Student student;
            for (const ScorePiece &score_piece : crs_iter->final_score())
            {
                auto stu_iter = manager_.FindStudent(score_piece.id);
                if (stu_iter == manager_.student_end())
                {
                    std::cout << "ERROR: Failed to find a student that should "
                                 "exist\n"
                                 "Course ID: " << id << "\n"
                                 "Student fail: " << score_piece.id << "\n"
                                 "Students now:\n";
                    ListStudents(args);
                    std::cout << "Courses now:\n";
                    ListCourses(args);

                    std::exit(EXIT_FAILURE);
                }
                std::cout << student.info().ToString() << std::endl;
            }
        }
        else
        {
            std::cout << "No course with ID " << id << std::endl;
        }
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: crs <ID>\n";
    }
}

void CommandLineInterface::RegisterToCourse(const std::string &args)
{
    std::istringstream iss(args);

    Student::IDType student_id;
    Course::IDType course_id;

    if (iss >> student_id >> course_id)
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

void CommandLineInterface::DropFromCourse(const std::string &args)
{
    std::istringstream iss(args);

    Student::IDType student_id;
    Course::IDType course_id;

    if (iss >> student_id >> course_id)
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

void CommandLineInterface::Save(const std::string &args)
{
    ManagerWriter writer;

    if (writer.Write("students.dat", "courses.dat", manager_))
        std::cout << "Saved\n";
    else
        std::cout << "Failed to save\n";
}


void CommandLineInterface::Load(const std::string &args)
{
    ManagerReader reader;

    if (reader.Read("students.dat", "courses.dat", manager_))
        std::cout << "Loaded\n";
    else
        std::cout << "Failed to load\n";
}

}  // namespace SAM

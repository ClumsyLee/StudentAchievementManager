#include <iostream>
#include <sstream>

#include "command_line_interface.h"

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
              {"drop", &CommandLineInterface::DropFromCourse}
          },
          prompt_("SAM: "),
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


void CommandLineInterface::ListStudents(const std::string &command)
{
    for (auto iter = manager_.student_begin(); iter != manager_.student_end();
         ++iter)
    {
        PrintStudentInfo(std::cout, iter->second.info());
    }
}

void CommandLineInterface::AddStudent(const std::string &command)
{
    std::istringstream iss(command);

    StudentInfo info;

    if (iss >> info.id >> info.name >> info.is_male)
    {
        if (!manager_.AddStudent(info))
            std::cout << "Fail to add student: ID " << info.id
                      << " has been occupied\n";
    }
    else
    {
        std::cout << "Syntax error\n"
                     "Usage: add-stu <ID> <name> <is male>\n";
    }
}

void CommandLineInterface::RemoveStudent(const std::string &command)
{
    std::istringstream iss(command);

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

void CommandLineInterface::ShowStudent(const std::string &command)
{
    std::istringstream iss(command);

    StudentInfo::IDType id;
    if (iss >> id)
    {
        Student student;
        if (manager_.SearchStudent(id, student))
        {
            PrintStudentInfo(std::cout, student.info());
            std::cout << "Course taken:\n";

            Course course;
            for (const Course::IDType &course_id : student.courses_taken())
            {
                if (!manager_.SearchCourse(course_id, course))
                {
                    std::cout << "ERROR: Failed to find a course that should"
                                 "exist\n"
                                 "Student ID: " << id << "\n"
                                 "Course fail: " << course_id << "\n"
                                 "Students now:\n";
                    ListStudents(command);
                    std::cout << "Courses now:\n";
                    ListCourses(command);

                    std::exit(EXIT_FAILURE);
                }
                PrintCourseInfo(std::cout, course.info());
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

void CommandLineInterface::ListCourses(const std::string &command)
{
    for (auto iter = manager_.course_begin(); iter != manager_.course_end();
         ++iter)
    {
        PrintCourseInfo(std::cout, iter->second.info());
    }
}

void CommandLineInterface::AddCourse(const std::string &command)
{
    std::istringstream iss(command);

    CourseInfo info;

    iss >> info.id >> info.name >> info.credit >> info.teacher_name;
    if (!iss)
    {
        std::cout << "Syntax error\n"
                  << "Usage: add-crs <ID> <name> <credit> <teacher name>\n";
    }
    else
    {
        if (!manager_.AddCourse(info))
            std::cout << "Fail to add course: ID " << info.id
                      << " has been occupied\n";
    }
}

void CommandLineInterface::RemoveCourse(const std::string &command)
{
    std::istringstream iss(command);

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

void CommandLineInterface::ShowCourse(const std::string &command)
{
    std::istringstream iss(command);

    CourseInfo::IDType id;
    if (iss >> id)
    {
        Course course;
        if (manager_.SearchCourse(id, course))
        {
            PrintCourseInfo(std::cout, course.info());
            std::cout << "Student(s) in this course:\n";

            Student student;
            for (const Student::IDType &student_id : course.StudentList())
            {
                if (!manager_.SearchStudent(student_id, student))
                {
                    std::cout << "ERROR: Failed to find a student that should"
                                 "exist\n"
                                 "Course ID: " << id << "\n"
                                 "Student fail: " << student_id << "\n"
                                 "Students now:\n";
                    ListStudents(command);
                    std::cout << "Courses now:\n";
                    ListCourses(command);

                    std::exit(EXIT_FAILURE);
                }
                PrintStudentInfo(std::cout, student.info());
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

void CommandLineInterface::RegisterToCourse(const std::string &command)
{
    std::istringstream iss(command);

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

void CommandLineInterface::DropFromCourse(const std::string &command)
{
    std::istringstream iss(command);

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


void CommandLineInterface::PrintStudentInfo(std::ostream &os,
                                            const StudentInfo &info)
{
    os << info.id << " "
       << info.name << " "
       << (info.is_male? "男": "女") << std::endl;
}

void CommandLineInterface::PrintCourseInfo(std::ostream &os,
                                           const CourseInfo &info)
{
    os << info.id << " "
       << info.name << " "
       << info.credit << " "
       << info.teacher_name << std::endl;
}


}  // namespace SAM

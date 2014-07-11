#include <iostream>
#include <sstream>

#include "command_line_interface.h"

namespace SAM {

CommandLineInterface::CommandLineInterface()
        : commands_{
              {"ls-stu", &CommandLineInterface::ListStudents},
              {"ls-crs", &CommandLineInterface::ListCourses},
              {"add-stu", &CommandLineInterface::AddStudent},
              {"add-crs", &CommandLineInterface::AddCourses}
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
        PrintStudent(std::cout, iter->second);
    }
}
void CommandLineInterface::ListCourses(const std::string &command)
{

}

void CommandLineInterface::AddStudent(const std::string &command)
{
    std::istringstream iss(command);

    StudentInfo info;

    iss >> info.id >> info.name >> info.is_male;
    manager_.AddStudent(info);
}

void CommandLineInterface::AddCourses(const std::string &command)
{

}

void CommandLineInterface::PrintStudent(std::ostream &os,
                                        const Student &student)
{
    const StudentInfo &info = student.info();
    os << info.id << " " << info.name << " " << info.is_male << std::endl;
}


}  // namespace SAM

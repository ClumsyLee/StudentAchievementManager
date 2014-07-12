#ifndef SAM_COMMAND_LINE_INTERFACE_H_
#define SAM_COMMAND_LINE_INTERFACE_H_

#include <functional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "interface.h"
#include "manager.h"

namespace SAM {

class CommandLineInterface : public Interface
{
 public:
    CommandLineInterface();
    virtual ~CommandLineInterface() = default;

    virtual int Run(int argc, const char* const argv[]);


 private:
    typedef std::function<void(CommandLineInterface &,
                               const std::string &)> ParseFunction;
    typedef std::pair<std::string, ParseFunction> Command;

    // return true if need to exit
    bool ParseAndRunCommand(const std::string &command);

    void ListStudents(const std::string &command);
    void AddStudent(const std::string &command);
    void RemoveStudent(const std::string &command);
    void ShowStudent(const std::string &command);


    void ListCourses(const std::string &command);
    void AddCourse(const std::string &command);
    void RemoveCourse(const std::string &command);
    void ShowCourse(const std::string &command);

    void RegisterToCourse(const std::string &command);
    void DropFromCourse(const std::string &command);

    void PrintHelpInfo() const;

    static void PrintStudentInfo(std::ostream &os, const StudentInfo &info);
    static void PrintCourseInfo(std::ostream &os, const CourseInfo &info);


    std::vector<Command> commands_;
    std::string prompt_;

    Manager manager_;
};

}  // namespace SAM

#endif  // SAM_COMMAND_LINE_INTERFACE_H_

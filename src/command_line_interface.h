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

    void ListStudents(const std::string &args);
    void AddStudent(const std::string &args);
    void RemoveStudent(const std::string &args);
    void ShowStudent(const std::string &args);


    void ListCourses(const std::string &args);
    void AddCourse(const std::string &args);
    void RemoveCourse(const std::string &args);
    void ShowCourse(const std::string &args);

    void RegisterToCourse(const std::string &args);
    void DropFromCourse(const std::string &args);

    void Save(const std::string &args);
    void Load(const std::string &args);

    void PrintHelpInfo() const;

    std::vector<Command> commands_;
    std::string prompt_;

    Manager manager_;
};

}  // namespace SAM

#endif  // SAM_COMMAND_LINE_INTERFACE_H_

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
    void ListCourses(const std::string &command);

    void AddStudent(const std::string &command);
    void AddCourses(const std::string &command);


    static void PrintStudent(std::ostream &os, const Student &student);

    std::vector<Command> commands_;
    std::string prompt_;

    Manager manager_;
};

}  // namespace SAM

#endif  // SAM_COMMAND_LINE_INTERFACE_H_

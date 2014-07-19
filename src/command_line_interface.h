#ifndef SAM_COMMAND_LINE_INTERFACE_H_
#define SAM_COMMAND_LINE_INTERFACE_H_

#include <functional>
#include <ostream>
#include <sstream>
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
    typedef std::function<void(CommandLineInterface &)> ParseFunction;
    typedef std::pair<std::string, ParseFunction> Command;

    // return true if need to exit
    bool ParseAndRunCommand();

    void ListStudents();
    void AddStudent();
    void RemoveStudent();
    void ShowStudent();

    void ListCourses();
    void AddCourse();
    void RemoveCourse();
    void ShowCourse();

    void RegisterToCourse();
    void DropFromCourse();

    void GenerateTranscript();

    void Save();
    void Load();

    void PrintHelpInfo() const;

    bool ReadLine(const std::string &prompt, std::string &line) const;

    static std::vector<Command> commands_;

    std::string prompt_;
    std::istringstream command_stream_;
    Manager manager_;
};

}  // namespace SAM

#endif  // SAM_COMMAND_LINE_INTERFACE_H_

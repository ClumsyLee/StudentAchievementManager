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

    void ListStudents() const;
    void AddStudent();
    void RemoveStudent();
    void ShowStudent() const;

    void ListCourses() const;
    void AddCourse();
    void RemoveCourse();
    void ShowCourse() const;

    void RegisterToCourse();
    void DropFromCourse();

    void RecordFinalScore();
    void RemoveFinalScore();
    void GetScore();
    void ChangeScore();

    void GenerateTranscript() const;

    void Save();
    void Load();

    void PrintHelpInfo() const;

    bool GetStudentID(const char *prompt, Student::IDType &id,
                      bool check = false) const;
    bool GetCourseID(const char *prompt, Course::IDType &id,
                     bool check = false) const;
    bool GetStudentInfo(StudentInfo &info) const;
    bool GetCourseInfo(CourseInfo &info) const;

    bool GetMenuChoice(int max_number, int &choice);

    bool ReadLine(const char *prompt, std::string &line) const;
    bool ReadLineIntoStream(const char *prompt) const;
    friend char * CommandGenerator(const char *text, int state);

    static std::vector<Command> commands_;

    std::string prompt_;
    mutable std::istringstream command_stream_;
    Manager manager_;

    bool interactive_mode;
};

}  // namespace SAM

#endif  // SAM_COMMAND_LINE_INTERFACE_H_

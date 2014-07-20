#include <cstdlib>
#include <cstring>

#include <iomanip>
#include <iostream>
#include <sstream>

#include <readline/readline.h>
#include <readline/history.h>

#include "analyser.h"
#include "command_line_interface.h"
#include "io.h"

namespace {

void StriptWhite(std::string &str)
{
    std::size_t begin = str.find_first_not_of(" \t");
    std::size_t end = str.find_last_not_of(" \t");

    if (begin == std::string::npos)  // str contains only white
        str.clear();
    else
        str = str.substr(begin, end - begin + 1);
}


}  // namespace


namespace SAM {

void InitializeReadline();
char ** CommandCompletion(const char *text, int start, int end);
char * CommandGenerator(const char *text, int state);


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

    {"gen-stu", &CommandLineInterface::GenerateTranscript},

    {"save", &CommandLineInterface::Save},
    {"load", &CommandLineInterface::Load}
};


CommandLineInterface::CommandLineInterface()
        : prompt_("SAM-0.3: "),
          command_stream_(),
          manager_(),
          interactive_mode(true)
{
}

int CommandLineInterface::Run(int argc, const char* const argv[])
{
    std::string command;

    InitializeReadline();

    while (ReadLine(prompt_, command))
    {
        command_stream_.clear();
        command_stream_.str(command);  // add this line to stream

        if (ParseAndRunCommand())
            return 0;
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
        // Enter interactive mode
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
                 << "\n所选课程:\n\n"
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
                PrintScore(cout, crs_iter->GetScore(id));

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
                 << "\n课内学生:\n\n"
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
                PrintScore(cout, score_piece.score);

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

void CommandLineInterface::GenerateTranscript()
{
    Student::IDType student_id;

    if (command_stream_ >> student_id)
    {
        Transcript transcript;
        Analyser analyser;

        if (!analyser.GenerateTranscript(manager_, student_id, transcript))
        {
            std::cout << "Failed to generate transcript\n";
        }
        else
        {
            std::cout << transcript;
        }
    }
    else
    {
        std::cout << "Syntax error\n"
                  << "Usage: gen-stu <student ID>\n";
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


bool CommandLineInterface::ReadLine(const std::string &prompt,
                                    std::string &line) const
{
    char *line_read = readline(prompt.c_str());
    if (!line_read)  // EOF
        return false;

    line.assign(line_read);

    StriptWhite(line);
    if (!line.empty())
        add_history(line_read);  // if not empty, add the origin line

    std::free(line_read);
    return true;
}



/* Tell the GNU Readline library how to complete.  We want to try to complete
   on command names if this is the first word in the line, or on filenames
   if not. */
void InitializeReadline()
{
    /* Allow conditional parsing of the ~/.inputrc file. */
    rl_readline_name = "SAM";

    /* Tell the completer that we want a crack first. */
    rl_attempted_completion_function = CommandCompletion;
}

/* Attempt to complete on the contents of TEXT.  START and END bound the
   region of rl_line_buffer that contains the word to complete.  TEXT is
   the word to complete.  We can use the entire contents of rl_line_buffer
   in case we want to do some simple parsing.  Return the array of matches,
   or NULL if there aren't any. */
char ** CommandCompletion(const char *text, int start, int end)
{
    char **matches;

    matches = NULL;

    /* If this word is at the start of the line, then it is a command
     to complete.  Otherwise it is the name of a file in the current
     directory. */
    if (start == 0)
        matches = rl_completion_matches(text, CommandGenerator);

    return matches;
}

/* Generator function for command completion.  STATE lets us know whether
   to start from scratch; without any state (i.e. STATE == 0), then we
   start at the top of the list. */
char * CommandGenerator(const char *text, int state)
{
    static int list_index, len;

    /* If this is a new word to complete, initialize now.  This includes
     saving the length of TEXT for efficiency, and initializing the index
     variable to 0. */
    if (!state)
    {
        list_index = 0;
        len = std::strlen(text);
    }

    const auto &commands = CommandLineInterface::commands_;
    static int command_num = commands.size();

    /* Return the next name which partially matches from the command list. */
    while (list_index < command_num - 1)
    {
        list_index++;
        const std::string &command_name = commands[list_index].first;

        if (command_name.compare(0, len, text) == 0)
        {
            char *name = new char[command_name.size() + 1];
            std::strcpy(name, command_name.c_str());
            return name;
        }
    }

    /* If no names matched, then return NULL. */
    return NULL;
}



}  // namespace SAM

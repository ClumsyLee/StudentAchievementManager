#include <cctype>
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

    {"record-final", &CommandLineInterface::RecordFinalScore},
    {"remove-final", &CommandLineInterface::RemoveFinalScore},
    {"ch-score", &CommandLineInterface::ChangeScore},

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
    if (argc >= 2 && std::strcmp(argv[1], "-no-interact") == 0)
    {
        interactive_mode = false;
        InitializeReadline();

        while (ReadLineIntoStream(prompt_.c_str()))
        {
            if (ParseAndRunCommand())
                return 0;
        }
    }
    else  // interactive mode
    {
        using std::cout;

        Load();
        while (true)
        {
            cout << "\n请选择你要进行的操作\n"
                    "1. 管理学生               2.管理课程\n"
                    "3. 退出\n";
            int choice;
            if (!GetMenuChoice(3, choice) || choice == 3)
                break;

            switch (choice)
            {
                case 1:
                {
                    while (true)
                    {
                        cout << "\n请选择你要对学生进行的操作\n"
                                "1. 显示学生列表            2. 添加学生\n"
                                "3. 移除学生                4. 查询特定学生\n"
                                "5. 将学生注册到课程        6. 退课\n"
                                "7. 生成学生成绩单          8. 修改分数\n"
                                "9. 返回上级菜单\n";
                        int choice;
                        if (!GetMenuChoice(9, choice) || choice == 9)
                            break;

                        switch (choice)
                        {
                            case 1: ListStudents(); break;
                            case 2: AddStudent(); break;
                            case 3: RemoveStudent(); break;
                            case 4: ShowStudent(); break;
                            case 5: RegisterToCourse(); break;
                            case 6: DropFromCourse(); break;
                            case 7: GenerateTranscript(); break;
                            case 8: ChangeScore(); break;
                        }
                    }
                    break;
                }
                case 2:
                {
                    while (true)
                    {
                        cout << "\n请选择你要对课程进行的操作\n"
                                "1. 显示课程列表            2. 添加课程\n"
                                "3. 移除课程                4. 查询特定课程\n"
                                "5. 记录期末成绩            6. 移除期末成绩\n"
                                "7. 返回上级菜单\n";
                        int choice;
                        if (!GetMenuChoice(7, choice) || choice == 7)
                            break;

                        switch (choice)
                        {
                            case 1: ListCourses(); break;
                            case 2: AddCourse(); break;
                            case 3: RemoveCourse(); break;
                            case 4: ShowCourse(); break;
                            case 5: RecordFinalScore(); break;
                            case 6: RemoveFinalScore(); break;
                        }
                    }
                    break;
                }
            }
        }
        Save();
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


void CommandLineInterface::ListStudents() const
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

    if (interactive_mode)
    {
        if (!GetStudentInfo(info))
            return;
    }
    else  // noninteractive
    {
        if (!MakeStudentInfo(command_stream_.str(), info))
        {
            std::cout << "Fail to construct student info from \""
                      << command_stream_.str() << "\"\n";
            return;
        }
    }

    if (!manager_.AddStudent(info))
    {
        std::cout << "无法添加新学生: ID " << info.id
                  << " 已经被占用\n";
    }
}

void CommandLineInterface::RemoveStudent()
{
    StudentInfo::IDType id;

    if (!GetStudentID("请输入要移除的学生的ID: ", id, true))
        return;

    if (!manager_.RemoveStudent(id))
        std::cerr << "Internal Error\n";
}

void CommandLineInterface::ShowStudent() const
{
    using std::cout;

    StudentInfo::IDType id;

    if (!GetStudentID("请输入要展示的学生的ID: ", id, true))
        return;


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
                std::cerr << "Internal Error\n";
                return;
            }

            cout << *crs_iter << ' ';

            // add score info to the end
            cout.width(kScoreWidth);
            PrintScore(cout, crs_iter->GetScore(id));

            cout << std::endl;
        }
    }
    else
    {
        std::cerr << "Internal Error\n";
        return;
    }
}

void CommandLineInterface::ListCourses() const
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

    if (interactive_mode)
    {
        if (!GetCourseInfo(info))
            return;
    }
    else  // noninteractive
    {
        if (!MakeCourseInfo(command_stream_.str(), info))
        {
            std::cout << "Fail to construct course info from \""
                      << command_stream_.str() << "\"\n";
            return;
        }
    }

    if (!manager_.AddCourse(info))
    {
        std::cout << "无法添加新课程: ID " << info.id << " 已经被占用\n";
    }
}

void CommandLineInterface::RemoveCourse()
{
    CourseInfo::IDType id;

    if (!GetCourseID("请输入要移除的课程的ID: ", id, true))
        return;

    if (!manager_.RemoveCourse(id))
        std::cerr << "Internal Error\n";
}

void CommandLineInterface::ShowCourse() const
{
    using std::cout;

    CourseInfo::IDType id;

    if (!GetCourseID("请输入要展示的课程的ID: ", id, true))
        return;

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
                std::cerr << "Internal Error\n";
                return;
            }
            cout << *stu_iter << ' ';

            cout.width(kScoreWidth);
            PrintScore(cout, score_piece.score);

            cout << std::endl;
        }
    }
    else
    {
        std::cerr << "Internal Error\n";
    }
}

void CommandLineInterface::RegisterToCourse()
{
    Student::IDType student_id;
    Course::IDType course_id;

    if (!GetStudentID("请输入要注册的学生的ID: ", student_id, true) ||
        !GetCourseID("请输入该学生要注册的课程的ID: ", course_id, true))
        return;

    if (!manager_.AddStudentToCourse(student_id, course_id))
    {
        std::cout << "无法将ID为 " << student_id << " 的学生注册到ID为 "
                  << course_id << " 的课程中\n"
                     "可能原因: 课程人数已满\n";
    }
}

void CommandLineInterface::DropFromCourse()
{
    Student::IDType student_id;
    Course::IDType course_id;

    if (!GetStudentID("请输入要退课的学生的ID: ", student_id, true) ||
        !GetCourseID("请输入该学生要退出的课程的ID: ", course_id, true))
        return;

    if (!manager_.RemoveStudentFromCourse(student_id, course_id))
    {
        std::cout << "无法将ID为 " << student_id << " 的学生从ID为 "
                  << course_id << "的课程中退课\n"
                     "可能原因: 该课程中没有该学生\n";
    }
}

void CommandLineInterface::RecordFinalScore()
{
    Course::IDType course_id;

    if (!GetCourseID("请输入要录入成绩的课程的ID: ", course_id, true))
        return;

    if (interactive_mode)
    {
        ReadLineIntoStream("请输入要录的成绩文件的文件名（空行以跳过）: ");
    }
    std::string filename = command_stream_.str();

    // read from file
    std::vector<Student::IDType> unscored_students;
    ManagerReader reader;
    if (!reader.ReadFinalScore(filename, manager_, course_id,
                               unscored_students) && !filename.empty())
    {
        std::cout << "无法从文件 " << filename << " 中读取考试成绩\n";
    }

    // record score for unscored_students
    for (Student::IDType student_id : unscored_students)
    {
        ScoreType score;
        std::string prompt = "请输入" + ShortStudentInfo(student_id) + "的成绩: ";
        if (!ReadLineIntoStream(prompt.c_str()))  // EOF
        {
            return;  // stop inputing right now
        }
        else if (command_stream_ >> score)
        {
            manager_.ChangeScore(student_id, course_id, score);
        }
        // else cannot read a score, skip this student
    }
}

void CommandLineInterface::RemoveFinalScore()
{
    Course::IDType course_id;

    if (!GetCourseID("请输入要移除期末成绩的课程的ID: ", course_id, true))
        return;

    std::string prompt = "确定要移除 " + ShortCourseInfo(course_id) +
                         " 的期末成绩吗 (y/n): ";
    if (GetYesNoChoice(prompt.c_str()))
        manager_.RemoveFinalScore(course_id);
}

void CommandLineInterface::ChangeScore()
{
    Student::IDType student_id;
    Course::IDType course_id;

    if (!GetStudentID("请输入要修改分数的学生的ID: ", student_id, true) ||
        !GetCourseID("请输入该学生要修改分数的课程的ID: ", course_id, true))
        return;

    if (interactive_mode)
    {
        std::cout << ShortStudentInfo(student_id) << "在课程"
                  << ShortCourseInfo(course_id) << "中的当前分数为: ";
        PrintScore(std::cout, manager_.GetScore(student_id, course_id)) << '\n';

        if (!ReadLineIntoStream("请输入修改后的分数: "))
            return;
    }

    ScoreType score;
    if (command_stream_ >> score)
    {
        if (manager_.ChangeScore(student_id, course_id, score))
            std::cout << "修改成功\n";
        else
            std::cerr << "Internal Error\n";
    }
}


void CommandLineInterface::GenerateTranscript() const
{
    Student::IDType student_id;

    if (!GetStudentID("请输入要生成成绩单的学生的ID: ", student_id, true))
        return;


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

bool CommandLineInterface::GetStudentID(const char *prompt,
                                        Student::IDType &id,
                                        bool check) const
{
    if (interactive_mode)
    {
        if (!ReadLineIntoStream(prompt))
            return false;
    }

    if (!(command_stream_ >> id))
    {
        std::cout << command_stream_.str() << ": 无效的学生ID\n";
        return false;
    }

    if (check)  // need to check whether the student is in the manager
    {
        if (!manager_.HasStudent(id))
        {
            std::cout << "不存在ID为 " << id << " 的学生\n";
            return false;
        }
    }

    return true;
}

bool CommandLineInterface::GetCourseID(const char *prompt,
                                       Course::IDType &id,
                                       bool check) const
{
    if (interactive_mode)
    {
        if (!ReadLineIntoStream(prompt))
            return false;
    }

    if (!(command_stream_ >> id))
    {
        std::cout << command_stream_.str() << ": 无效的课程号\n";
        return false;
    }


    if (check)  // need to check whether the course is in the manager
    {
        if (!manager_.HasCourse(id))
        {
            std::cout << "不存在课程号为 " << id << " 的课程\n";
            return false;
        }
    }

    return true;
}


bool CommandLineInterface::GetStudentInfo(StudentInfo &info) const
{
    // name
    while (true)
    {
        if (!ReadLineIntoStream("请输入学生ID: "))
            return false;

        if (!(command_stream_ >> info.id))
            std::cout << "无效的学生ID\n";
        else if (manager_.HasStudent(info.id))
            std::cout << "ID " << info.id << "已经被占用\n";
        else
            break;
    }

    // name
    while (true)
    {
        if (!ReadLineIntoStream("请输入学生姓名: "))
            return false;

        if (!(command_stream_ >> info.name))
            std::cout << "无效的姓名\n";
        else
            break;
    }

    // is_male
    while (true)
    {
        if (!ReadLineIntoStream("请输入学生性别（0代表女，1代表男）: "))
            return false;

        if (!(command_stream_ >> info.is_male))
            std::cout << "无效的性别\n";
        else
            break;
    }

    // department
    ShowDepartments(std::cout);
    while (true)
    {
        if (!ReadLineIntoStream("请输入学生的专业编号: "))
            return false;

        if (!(command_stream_ >> info.department))
            std::cout << "无效的专业编号\n";
        else
            break;
    }

    return true;
}

bool CommandLineInterface::GetCourseInfo(CourseInfo &info) const
{
    // name
    while (true)
    {
        if (!ReadLineIntoStream("请输入课程ID: "))
            return false;

        if (!(command_stream_ >> info.id))
            std::cout << "无效的课程ID\n";
        else if (manager_.HasCourse(info.id))
            std::cout << "ID " << info.id << "已经被占用\n";
        else
            break;
    }

    // name
    while (true)
    {
        if (!ReadLineIntoStream("请输入课程名: "))
            return false;

        if (!(command_stream_ >> info.name))
            std::cout << "无效的课程名\n";
        else
            break;
    }

    // department
    ShowDepartments(std::cout);
    while (true)
    {
        if (!ReadLineIntoStream("请输入课程的专业编号: "))
            return false;

        if (!(command_stream_ >> info.department))
            std::cout << "无效的专业编号\n";
        else
            break;
    }

    // credit
    while (true)
    {
        if (!ReadLineIntoStream("请输入课程学分: "))
            return false;

        if (!(command_stream_ >> info.credit))
            std::cout << "无效的学分\n";
        else
            break;
    }

    // capacity
    while (true)
    {
        if (!ReadLineIntoStream("请输入课容量: "))
            return false;

        if (!(command_stream_ >> info.capacity))
            std::cout << "无效的课容量\n";
        else
            break;
    }

    // teacher name
    while (true)
    {
        if (!ReadLineIntoStream("请输入老师姓名: "))
            return false;

        if (!(command_stream_ >> info.teacher_name))
            std::cout << "无效的老师姓名\n";
        else
            break;
    }

    return true;
}

bool CommandLineInterface::GetMenuChoice(int max_number, int &choice)
{
    if (!ReadLineIntoStream(NULL))
        return false;

    while (!(command_stream_ >> choice) || choice < 1 || choice >max_number)
    {
        std::string prompt("请输入不超过" + std::to_string(max_number) +
                           "的正整数: ");
        if (!ReadLineIntoStream(prompt.c_str()))
            return false;
    }
    return true;
}

bool CommandLineInterface::GetYesNoChoice(const char *prompt) const
{
    while (ReadLineIntoStream(prompt))
    {
        char choice;
        if (command_stream_ >> choice)
        {
            choice = std::tolower(choice);
            if (choice == 'y')
                return true;
            else if (choice == 'n')
                return false;
        }
    }
    return false;
}



bool CommandLineInterface::ReadLine(const char *prompt,
                                    std::string &line) const
{
    char *line_read = readline(prompt);
    if (!line_read)  // EOF
        return false;

    line.assign(line_read);

    StriptWhite(line);
    if (!line.empty())
        add_history(line_read);  // if not empty, add the origin line

    std::free(line_read);
    return true;
}

bool CommandLineInterface::ReadLineIntoStream(const char *prompt) const
{
    std::string line;
    if (!ReadLine(prompt, line))
    {
        return false;
    }
    else
    {
        command_stream_.clear();
        command_stream_.str(line);  // add this line to stream
        return true;
    }
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
    while (list_index < command_num)
    {
        const std::string &command_name = commands[list_index].first;
        list_index++;

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

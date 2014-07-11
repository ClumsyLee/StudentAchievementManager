#ifndef SAM_TEXT_INTERFACE_H_
#define SAM_TEXT_INTERFACE_H_

#include <functional>
#include <istream>
#include <string>
#include <vector>

#include <curses.h>

#include "interface.h"
#include "manager.h"

namespace SAM {

class TextInterface : Interface
{
 public:
    TextInterface();
    virtual ~TextInterface();

    virtual int Run(int argc, const char* const argv[]);

 private:
    struct MenuEntry
    {
        std::string name;
        void (TextInterface::*function)();
    };

    static int ComputeMenuWidth(const std::string &title,
                                const std::vector<MenuEntry> &choices);

    void GetIntoMenu(
            const std::string &title,
            const std::vector<MenuEntry> &choices);
    void PrintMenu(WINDOW *menu_win,
                   const std::string &title,
                   const std::vector<MenuEntry> &choices,
                   int choice_now) const;
    void ManageStudents();
    void ManageCourse();

    void ShowAllStudents();
    void PrintStudents(
            std::map<Student::IDType, Student>::const_iterator head_iter,
            int choice_now) const;

    void ShowStudentDetail(const Student &student);

    Manager manager_;
};

}  // namespace SAM

#endif  // SAM_TEXT_INTERFACE_H_

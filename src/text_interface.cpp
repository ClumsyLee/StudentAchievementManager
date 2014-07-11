#include <locale.h>
#include <iostream>
#include <curses.h>
#include "text_interface.h"

namespace SAM {

TextInterface::TextInterface() : manager_()
{
    setlocale(LC_ALL, "");
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    curs_set(0);  // make curse invisible
                  // remember to make it visible when requiring an input
    noecho();  // same as the last one
}

TextInterface::~TextInterface()
{
    endwin();
}

int TextInterface::Run(int argc, const char* const argv[])
{
    GetIntoMenu("主界面",
                {{"管理学生I LOVE XD", &TextInterface::ManageStudents},
                 {"管理课程", &TextInterface::ManageCourse}});
    return 0;
}

void TextInterface::GetIntoMenu(
        const std::string &title,
        const std::vector<MenuEntry> &choices)
{
    int choice_now = 0;
    int input_char;

    int window_height = choices.size() + 6;
    int window_width = ComputeMenuWidth(title, choices);
    int starty = (LINES - window_height) / 2;
    int startx = (COLS - window_width) / 2;

    WINDOW *menu_win = newwin(window_height, window_width, starty, startx);
    keypad(menu_win, TRUE);

    PrintMenu(menu_win, title, choices, choice_now);

    while ((input_char = wgetch(menu_win)) != 'q')
    {
        switch (input_char)
        {
            case KEY_UP:
            {
                if (choice_now == 0)
                    choice_now = choices.size() - 1;
                else
                    choice_now--;

                break;
            }
            case KEY_DOWN:
            {
                if (choice_now == choices.size() - 1)
                    choice_now = 0;
                else
                    choice_now++;

                break;
            }
            case 10:
            {
                wclear(menu_win);
                wrefresh(menu_win);  // clear this menu window before move on

                (this->*(choices[choice_now].function))();

                break;
            }
            default:
            {
                continue;  // no need to print menu
            }
        }
        PrintMenu(menu_win, title, choices, choice_now);
    }

    wclear(menu_win);
    wrefresh(menu_win);  // clear the menu window before getting back
    delwin(menu_win);
}


int TextInterface::ComputeMenuWidth(const std::string &title,
                                    const std::vector<MenuEntry> &choices)
{
    int max_width = title.size();
    for (const MenuEntry &entry : choices)
    {
        int line_width = entry.name.size();
        if (line_width > max_width)
            max_width = line_width;
    }
    return max_width / 3 * 2 + 4;  // for Chinese
}


void TextInterface::PrintMenu(WINDOW *menu_win,
                              const std::string &title,
                              const std::vector<MenuEntry> &choices,
                              int choice_now) const
{
    int win_row, win_col;
    getmaxyx(menu_win, win_row, win_col);

    int row = 4;

    box(menu_win, 0, 0);

    mvwaddstr(menu_win, 2, (win_col - title.size() / 3 * 2) / 2,
              title.c_str());

    for (int i = 0; i < choices.size(); i++)
    {
        int col = (win_col - choices[i].name.size() / 3 * 2) / 2;  // for Chinese
        if (choice_now == i)
        {
            wattron(menu_win, A_REVERSE);
            mvwaddstr(menu_win, row, col, choices[i].name.c_str());
            wattroff(menu_win, A_REVERSE);
        }
        else
        {
            mvwaddstr(menu_win, row, col, choices[i].name.c_str());
        }
        row++;  // move to next line
    }
    wrefresh(menu_win);
}


void TextInterface::ManageStudents()
{
    GetIntoMenu("管理学生",
                {{"显示所有学生", &TextInterface::ShowAllStudents}});
}

void TextInterface::ManageCourse()
{
    GetIntoMenu("管理课程",
                {{"显示所有课程", &TextInterface::ShowAllStudents}});
}


void TextInterface::ShowAllStudents()
{
    int max_entry_number = LINES - 5;

    int head_now = 0;
    auto head_iter = manager_.students().begin();
    int choice_now = 0;
    auto choice_iter = head_iter;

    PrintStudents(head_iter, choice_now);

    int input_char;
    while ((input_char = getch()) != 'q')
    {
        switch (input_char)
        {
            case KEY_UP:
            {
                if (choice_now == 0)
                {
                    choice_now =  manager_.students().size() - 1;
                    choice_iter = manager_.students().end();
                }
                else
                {
                    --choice_now;
                    --choice_iter;
                }

                if (choice_now == head_now - 1)
                {
                    --head_now;
                    --head_iter;
                }
                else if (choice_now >= head_now + max_entry_number)
                {
                    head_now = choice_now - max_entry_number + 1;
                    head_iter = manager_.students().begin();
                    std::advance(head_iter, head_now);
                }
                break;
            }
            case KEY_DOWN:
            {
                if (choice_now == manager_.students().size() - 1)
                {
                    choice_now = 0;
                    choice_iter = manager_.students().begin();
                }
                else
                {
                    ++choice_now;
                    ++choice_iter;
                }

                if (choice_now == head_now + max_entry_number)
                {
                    ++head_now;
                    ++head_iter;
                }
                else if (choice_now < head_now)
                {
                    head_now = choice_now;
                    head_iter = manager_.students().begin();
                }
                break;
            }
            case 10:
            {
                clear();
                refresh();
                ShowStudentDetail(choice_iter->second);

                break;
            }
            default:
            {
                continue;  // no need to print menu
            }
        }
        PrintStudents(head_iter, choice_now);
    }

}

void TextInterface::PrintStudents(
        std::map<Student::IDType, Student>::const_iterator head_iter,
        int choice_now) const
{
    mvaddstr(1, 1, "删除学生（backspace）");

    int y = 2;
    int x = 1;


}

void TextInterface::ShowStudentDetail(const Student &student)
{

}


}

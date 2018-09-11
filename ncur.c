#include <ncurses.h>
#include <form.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);

int main ()
{
    WINDOW *my_win;
    FIELD *field[3];
    FORM *my_form;
    int height = 10;
    int width = 20;
    int starty = (LINES - height)/2;
    int startx = (COLS - width)/2;
    int ch;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    

    /** colors */
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);

    field[0] = new_field(1, 10, 4, 18, 0, 0);
    field[1] = new_field(1, 10, 6, 18, 0, 0);
    field[2] = NULL;
    

    set_field_fore(field[0], COLOR_PAIR(1));
    set_field_back(field[0], COLOR_PAIR(2));
    field_opts_off(field[0], O_AUTOSKIP);

    set_field_back(field[1], A_UNDERLINE);
    field_opts_off(field[1], O_AUTOSKIP);

    my_form = new_form(field);
    post_form(my_form);
    refresh();

    mvprintw(4, 10, "Value 1:");
    mvprintw(6, 10, "Value 2:");
    refresh();

    /*printw("Press F10 to exit!!");*/
    refresh();
    my_win = create_newwin(height, width, starty, startx);

    while((ch = getch()) != KEY_F(10))
    {
        /*destroy_win(my_win);*/
        /*my_win = create_newwin(height, width, starty, startx);*/
        form_driver(my_form, ch);
    }

    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);

    endwin();
    
    return 0;
}

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wborder(local_win, '@', '@', '=', '=', '+', '+', '+', '+');
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    wborder(local_win, ' ',' ',' ',' ',' ',' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}

#include <ncurses.h>
#include <iostream>
#include <stdio.h>
#include <string>


using namespace std;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
int main(int argc, char *argv[])
{	
	WINDOW *my_win;
	//int startx, starty, width, height;
	int ch;


	initscr();			/* Start curses mode 		*/
	cbreak();			/* Line buffering disabled, Pass					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	int height = 3;
	int width = 10;
	int starty = (LINES - height)/2;
	int startx = (COLS - width)/2;
	//height = 3;
	//width = 10;
	//starty = (LINES - height) / 2;	[> Calculating for a center placement <]
	//startx = (COLS - width) / 2;	[> of the window		<]
	printw("Press F1 to exit");
	refresh();
	my_win = create_newwin(height, width, starty, startx);

	while((ch = getch()) != KEY_F(10))
	{	switch(ch)
		{	case KEY_LEFT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,--startx);
				break;
			case KEY_RIGHT:
				destroy_win(my_win);
				my_win = create_newwin(height, width, starty,++startx);
				break;
			case KEY_UP:
				destroy_win(my_win);
				my_win = create_newwin(height, width, --starty,startx);
				break;
			case KEY_DOWN:
				destroy_win(my_win);
				my_win = create_newwin(height, width, ++starty,startx);
				break;	
		}
	}
		
    destroy_win(my_win);
	endwin();			/* End curses mode		  */
	return 0;
}
//int main ()
//{
    //WINDOW *my_win;
    //int height = 30;
    //int width = 100;
    //int starty = (LINES - height)/2;
    //int startx = (COLS - width)/2;
    //int ch;

    //initscr();
    //cbreak();
    //keypad(stdscr, TRUE);
    

    //printw("Press F10 to exit!!");
    //refresh();
    //my_win = create_newwin(height, width, starty, startx);

	//while((ch = getch()) != KEY_F(10)){
		//destroy_win(my_win);
		//my_win = create_newwin(height, width, starty, startx);
	//}

		//destroy_win(my_win);
    //endwin();
    
    //return 0;
//}
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

void forms()
{
    FIELD *field[4];
    FORM *my_form;
    int ch;

    /** colors */
    init_pair(1, COLOR_WHITE, COLOR_CYAN);
    init_pair(2, COLOR_WHITE, COLOR_CYAN);

    field[0] = new_field(1, 10, 4, 18, 0, 0);
    field[1] = new_field(1, 10, 6, 18, 0, 0);
    field[2] = new_field(1, 10, 8, 18, 0, 0);
    field[3] = NULL;

    set_field_fore(field[0], COLOR_PAIR(1));
    set_field_back(field[0], COLOR_PAIR(2));
    field_opts_off(field[0], O_AUTOSKIP);

    set_field_back(field[1], A_UNDERLINE);
    field_opts_off(field[1], O_AUTOSKIP);

    set_field_back(field[2], A_UNDERLINE);
    field_opts_off(field[2], O_AUTOSKIP);

    my_form = new_form(field);
    post_form(my_form);
    refresh();

    set_current_field(my_form, field[0]);
    mvprintw(4, 10, "Value 1:");
    mvprintw(6, 10, "Value 2:");
    mvprintw(8, 10, "Value3");
    mvprintw(LINES - 2, 0, "Use UP, DOWN");
    //mvprintw(12, 10, buffer.GetString());
    //mvprintw(10, 10, readBuffer.c_str());

    //std::string str = "test";    //mvprintw(10, 10, str.c_str());
    //Value::MemberIterator issues = d["issues"];

    while((ch = getch()) != KEY_F(10))
    {
        switch(ch){
            case KEY_DOWN:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                mvprintw(LINES -4, 0, "%s", field_buffer(field[0], 0));
                break;
            case KEY_UP:
                form_driver(my_form, REQ_PREV_FIELD);
                form_driver(my_form, REQ_END_LINE);
                break;
             // press Enter key
            case 10:
                form_driver(my_form, REQ_NEXT_FIELD);
                form_driver(my_form, REQ_END_LINE);
                mvprintw(LINES -4, 0, "%s", field_buffer(field[0], 0));
                break;
            default:
               form_driver(my_form, ch);
               break;
        }

        refresh();
    }

    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);
    free_field(field[2]);
}

void panels()
{
    int count = 1;
    WINDOW *my_wins[count];
    PANEL *my_panels[count];
    int lines = 10,
        cols = 40,
        y = 2,
        x = 4,
        i;
    my_wins[0] = newwin(lines, cols, y, x);

    mvwprintw(my_wins[0], y, x, "%s", "hi there!");
    wrefresh(my_wins[0]);

    //my_wins[1] = newwin(lines, cols, y + 1, x + 5);
    //my_wins[2] = newwin(lines, cols, y + 2, x + 10);

        box(my_wins[0], 0, 0);
    //for(i = 0; i < 3; ++i){
        //box(my_wins[i], 0, 0);
    //}

    my_panels[0] = new_panel(my_wins[0]);
    //my_panels[1] = new_panel(my_wins[1]);
    //my_panels[2] = new_panel(my_wins[2]);

    update_panels();
    doupdate();
}

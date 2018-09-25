#include <locale.h>
//#include "ncursesw/curses.h"
#include <ncurses.h>
#include <form.h>
#include <panel.h>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include <curl/curl.h>
#include <stdio.h>
#include <string>
#include <sstream>

//jira board backlog -b 183 -a y.fedoruk
//curl -X GET  -k 'https://jira.favorit/rest/agile/1.0/board/183/backlog?jql=assignee=y.fedoruk'   -H 'Authorization: Basic eS5mZWRvcnVrOkE4eWZlZG9ydWNr'   -H 'Cache-Control: no-cache'   -H 'Content-Type: application/json'
using namespace rapidjson;
using namespace std;

//void forms();

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
std::string request();
std::string wrap(std::string text, size_t line_length = 72);
std::string textWrap(std::string str, int location);

Document parse(const char* str);
void panels();
PANEL* cell(const char* s, int l, int c, int y, int x);

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main ()
{
    setlocale(LC_ALL, "");
    WINDOW *my_win;

    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    int height = 10;
    int width = 40;
    //int starty = (LINES - height);
    //int startx = (COLS - width)/2;
    int starty = 0; //(LINES - height);
    int startx = 0;
    int ch;

    init_pair(1, COLOR_RED, COLOR_YELLOW);
    init_pair(4, COLOR_BLUE, COLOR_GREEN);

    std::string readBuffer = request();
    const char* json = readBuffer.c_str();
    Document document = parse(json);
    const Value& issues = document["issues"];

    //std::string str = "test";    //mvprintw(10, 10, str.c_str());
    //Value::MemberIterator issues = d["issues"];

        PANEL* key_panel = cell(issues[0]["key"].GetString(), 3, 15, 0, 2);
        PANEL* key_panels[3];
    // ++++++++++++++++++ PARSE DOM
    for (SizeType i = 0; i < issues.Size(); i++) {
        const Value &issue = issues[i];
        key_panels[i] = cell(issue["key"].GetString(), 3, 15, i*3, 2);
                    attron(COLOR_PAIR(1));
        cell(issue["fields"]["summary"].GetString(), 3, 181, i*3, 20);
                    attroff(COLOR_PAIR(1));
        //set_panel_userptr(key_panel, summary_panel);

        //mvprintw(12 + i, 10, issue["key"].GetString());
        //for (SizeType j = 0; j < issue["fields"]["summary"].Size(); j++) {
            //const Value &fields = issues[j]["fields"];
            //mvprintw(12 + i, 10, fields[j].GetString());
            //fields["summary"]
        //}
    }

    set_panel_userptr(key_panels[0], key_panels[1]);
    set_panel_userptr(key_panels[1], key_panels[2]);
    set_panel_userptr(key_panels[2], key_panels[0]);

        update_panels();
        doupdate();
    refresh();
    //my_win = create_newwin(height, width, starty, startx);
    //mvwprintw(my_win, starty + 1, startx + 2, "hi there!");
    //wrefresh(my_win);

    PANEL* top = key_panels[0];
    PANEL* top_bkp = key_panels[0];
    top_panel(top);

    WINDOW* topwin = panel_window(top);
        update_panels();
        doupdate();
    int topch;
    int ch1;
    WINDOW* issue_win;
    PANEL* issue_panel;
    //const char* description;
    std::string description;
    int row, col;
    while((ch = getch()) != 'q')
    {
        //topch = wgetch(topwin);
        switch(ch){
            case 9 :
                top = (PANEL*)panel_userptr(top);
                top_bkp = top;
                top_panel(top);
                //topwin = panel_window(top);
                    //attron(COLOR_PAIR(1));
                    //mvwprintw(topwin, 1, 2, "WWW11");
                    //attroff(COLOR_PAIR(1));
                break;
            case 10 :
                //mvwprintw(topwin, 1, 2, "OOOO");
                //hide_panel(top);
                issue_win = newwin(50, 165, 0, 0);
                getmaxyx(issue_win, row, col);
                //issue_win = newpad(30, 150);
                issue_panel = new_panel(issue_win);

                //show_panel(issue_panel);

                //description = "AAsdfffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffQQ\n123456";
                description = issues[0]["fields"]["description"].GetString();
                //description = textWrap(description, 50);
                description = wrap(description, 90);
                mvwprintw(issue_win, 1, 1, description.c_str());
                box(issue_win, 0, 0);
                wrefresh(issue_win);
                //mvwprintw(issue_win, 1, 4, description.c_str());
                //prefresh(issue_win, 0,0, 2,2, 130, 150);

                top_panel(issue_panel);
                while((ch1 = wgetch(issue_win)) != 'q'){
                    //...wait
                }

                //top_panel(top_bkp);
                hide_panel(issue_panel);
                break;
        }
        update_panels();
        doupdate();
    }

    delwin(issue_win);
    del_panel(issue_panel);
    endwin();
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
    wrefresh(local_win);

    return local_win;
}

void destroy_win(WINDOW *local_win)
{
    wborder(local_win, ' ',' ',' ',' ',' ',' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}

Document parse(const char* str)
{
    //const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    //StringBuffer buffer;
    //Writer <StringBuffer> writer(buffer);
    //document.Accept(writer);
    //mvprintw(8, 10, buffer.GetString());
    //mvprintw(12, 10, buffer.GetString());

    const char* json = str;
    Document document;
    document.Parse(json);

    return document;
}
std::string request()
{
    // curl query
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://jira.favorit/rest/agile/1.0/board/183/backlog?jql=assignee=y.fedoruk");
        list = curl_slist_append(list, "Authorization: Basic eS5mZWRvcnVrOkE4eWZlZG9ydWNr");
        list = curl_slist_append(list, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        //curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, FALSE);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK){
            mvprintw(8, 12, "error!!!!");
        }
        curl_slist_free_all(list);
        curl_easy_cleanup(curl);
        //mvprintw(8, 10, readBuffer.c_str());
    }
    return readBuffer;
}

std::string wrap(std::string text, size_t line_length)
{
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;
 
    if (words >> word) {
        wrapped << word;
        size_t space_left = line_length - word.length();
        while (words >> word) {
            if (space_left < word.length() + 1) {
                wrapped << '\n' << ' ' << word;
                space_left = line_length - word.length();
            } else {
                wrapped << ' ' << word;
                space_left -= word.length() + 1;
            }
        }
    }
    return wrapped.str();
}

std::string textWrap(std::string str, int location) {
    int n = str.rfind(' ', location);
    if (n != std::string::npos) {
        str.at(n) = '\n';
    }

    return str;
}

PANEL* cell(const char* str, int l, int c, int y, int x)
{
    WINDOW *my_cell;
    PANEL *my_panel;
    int lines = l,
        cols = c,
        yRel = 1,
        xRel = 2,
        i;
    my_cell = newwin(lines, cols, y, x);

    mvwprintw(my_cell, yRel, xRel, str);
    wrefresh(my_cell);
    box(my_cell, 0, 0);
    my_panel = new_panel(my_cell);
    //set_panel_userptr(my_panel, p);

    update_panels();
    doupdate();

    return my_panel;
}


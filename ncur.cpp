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


//jira board backlog -b 183 -a y.fedoruk
//curl -X GET  -k 'https://jira.favorit/rest/agile/1.0/board/183/backlog?jql=assignee=y.fedoruk'   -H 'Authorization: Basic eS5mZWRvcnVrOkE4eWZlZG9ydWNr'   -H 'Cache-Control: no-cache'   -H 'Content-Type: application/json'
using namespace rapidjson;
using namespace std;

//void forms();

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
std::string request();
Document parse(const char* str);
void panels();
void cell();

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main ()
{
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

    std::string readBuffer = request();
    const char* json = readBuffer.c_str();
    Document document = parse(json);
    const Value& issues = document["issues"];

    //panels();
    cell();
    //std::string str = "test";    //mvprintw(10, 10, str.c_str());
    //Value::MemberIterator issues = d["issues"];

    // ++++++++++++++++++ PARSE DOM
    for (SizeType i = 0; i < issues.Size(); i++) {
        const Value &issue = issues[i];
        mvprintw(12 + i, 10, issue["key"].GetString());
        mvprintw(12 + 2*i + 1, 10, issue["fields"]["summary"].GetString());

        //for (SizeType j = 0; j < issue["fields"]["summary"].Size(); j++) {
            //const Value &fields = issues[j]["fields"];
            //mvprintw(12 + i, 10, fields[j].GetString());
            //fields["summary"]
        //}

    }

    refresh();
    //my_win = create_newwin(height, width, starty, startx);
    //mvwprintw(my_win, starty + 1, startx + 2, "hi there!");
    //wrefresh(my_win);

    while((ch = getch()) != KEY_F(10))
    {
        refresh();
        //destroy_win(my_win);
        //my_win = create_newwin(height, width, starty, startx);
    }



    //destroy_win(my_win);
    endwin();
    
    return 0;
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

void cell()
{
    int count = 1;
    WINDOW *my_cell;
    PANEL *my_panel;
    int lines = 10,
        cols = 40,
        y = 2,
        x = 4,
        i;
    my_cell = newwin(lines, cols, y, x);

    mvwprintw(my_cell, y, x, "%s", "hi there!");
    wrefresh(my_cell);
    box(my_cell, 0, 0);
    my_panel = new_panel(my_cell);

    update_panels();
    doupdate();
}


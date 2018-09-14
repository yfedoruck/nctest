#include <ncurses.h>
#include <form.h>
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

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);


static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main ()
{
    WINDOW *my_win;
    FIELD *field[4];
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
    

    const char* json = "{\"project\":\"rapidjson\",\"stars\":10}";
    Document document;
    document.Parse(json);
    
    StringBuffer buffer;
    Writer <StringBuffer> writer(buffer);
    document.Accept(writer);
    mvprintw(8, 10, "Value3");
    //mvprintw(8, 10, buffer.GetString());
    //mvprintw(12, 10, buffer.GetString());
    refresh();
    

    // curl query
    CURL *curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    struct curl_slist *list = NULL;
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://jira.favorit/rest/agile/1.0/board/183/backlog?jql=assignee=y.fedoruk");
        list = curl_slist_append(list, "Authorization: Basic eS5mZWRvcnVrOkE4eWZlZG9ydWNr");
        list = curl_slist_append(list, "'Content-Type: application/json");
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
    mvprintw(LINES - 2, 0, "Use UP, DOWN");
    //mvprintw(12, 10, buffer.GetString());
    //mvprintw(10, 10, readBuffer.c_str());

    //std::string str = "test";    //mvprintw(10, 10, str.c_str());
    json = readBuffer.c_str();
    document.Parse(json);
    //Value::MemberIterator issues = d["issues"];
    const Value& issues = document["issues"];

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

    //mvprintw(12, 10, std::to_string(issues.Size()));
    //mvprintw(12, 10, issues.GetString());

    //mvprintw(12, 10, d["expand"].GetString());
    refresh();

    /*printw("Press F10 to exit!!");*/
    refresh();
    my_win = create_newwin(height, width, starty, startx);

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

        /*destroy_win(my_win);*/
        /*my_win = create_newwin(height, width, starty, startx);*/
    }

    unpost_form(my_form);
    free_form(my_form);
    free_field(field[0]);
    free_field(field[1]);
    free_field(field[2]);

    destroy_win(my_win);
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

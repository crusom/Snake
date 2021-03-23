#include <panel.h>
#include <ncurses.h>
#include <string>
#include <time.h>
#include <deque>

int xmove=0, ymove=0, xfood, yfood;
std::string lastkey="r";
std::deque<std::string> moves_queue = {lastkey};

void snakeFood(int &height, int &width){
    srand(time(NULL));
    xfood=rand()%width;
    yfood=rand()%height;
}

void snakeMove(int &height, int &width){
    int x=0,y=0;
    moves_queue.pop_back();
    moves_queue.push_front(lastkey);

    attron(COLOR_PAIR(1));
    mvprintw(yfood, xfood, "%s", "X");
    attroff(COLOR_PAIR(1));

    if(lastkey == "r") xmove+=1;
    else if(lastkey=="l") xmove-=1;
    else if(lastkey=="u") ymove-=1;
    else if(lastkey=="d") ymove+=1;
    /* End game when snake touch a border */
    if(height/2+ymove>=height || height/2+ymove<0 ||
        width/2+xmove>=width || width/2+xmove<0){
        printf("points: %d", moves_queue.size()*10); endwin(); std::exit(0);
    }
    /* Check if snake eat food */
    if(xfood==width/2+xmove && yfood==height/2+ymove){
        moves_queue.push_back(moves_queue.back());
        snakeFood(height, width);
    }
    /* Print snake's head, and then body in loop*/
    attron(COLOR_PAIR(2));
    mvprintw(height/2+ymove+y, width/2+xmove+x, "%s", "O");
    for (auto move : moves_queue) {
            if(move == "r") x-=1;  //right
            else if(move=="l") x+=1; //left
            else if(move=="u") y+=1; //up
            else if(move=="d") y-=1; //down
            /* if snake's head touch his tail */
            if(y==0 && x==0){
                printf("points: %d", moves_queue.size()*10); endwin(); std::exit(0);
            }

            mvprintw(height/2+ymove+y, width/2+xmove+x, "%s", "O");
        }
    attroff(COLOR_PAIR(2));
}

int main(){
    WINDOW *my_window;
    PANEL  *my_panel;
    int height, width;
    /* Initialize screen */
    initscr();
    /* Hide cursor */
    curs_set(0);
    start_color();
    init_color(COLOR_RED, 252, 28, 12);
    init_color(COLOR_GREEN, 35, 196, 17);
    init_pair(1, COLOR_RED, COLOR_RED);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    /* Get height and width */
    getmaxyx(stdscr,height,width);
    cbreak();
    noecho();
    /* Non blocking getch() */
    nodelay(stdscr, TRUE);

    my_window = newwin(height, width, 0, 0);
    keypad(stdscr, TRUE);

    my_panel = new_panel(my_window);
    update_panels();
    doupdate();
    /* snake delay */
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000;
    /* create snake food */
    snakeFood(height, width);
    int ch;

    while((ch = getch()) != KEY_F(1)){

        if(ch==KEY_UP && lastkey!="u" && lastkey!="d") lastkey = "u";
        if(ch==KEY_DOWN && lastkey!="u" && lastkey!="d") lastkey = "d";
        if(ch==KEY_LEFT && lastkey!="r" && lastkey!="l") lastkey = "l";
        if(ch==KEY_RIGHT && lastkey!="r" && lastkey!="l") lastkey = "r";

        clear();
        snakeMove(height, width);

        refresh();
        nanosleep(&ts, NULL);
    }

    endwin();
    return 0;
}

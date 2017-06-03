#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>
#include <string>
using namespace std;

int height,width,xo,yo,n=0,m=0;
WINDOW *menu;
WINDOW *plane;
WINDOW *list;
WINDOW *footer;

void drawmenu(int, int);
class Node
{
public:
  double x,y;
  bool active=false;
};

class Link
{
public:
  int one,two;
  bool active=false;
};

Node nodes[60];
Link links[120];

void addnode(){
  drawmenu(1,1);
  MEVENT event;
  mousemask(BUTTON1_CLICKED,NULL);
  bool loop=true,exist=false;
  while(loop==true){
    mvwprintw(footer,0,0,"Click to add node"); wrefresh(footer);
    if( wgetch(plane) == KEY_MOUSE )
    {
      getmouse(&event);
      if((event.y==1) && (event.x>width-10)){
        loop=false;
      }
      if(event.x>3 && event.x<width-20 && event.y>3 && event.y<width-1){

        //mvprintw(event.y,event.x,'ACS_CKBOARD');
        //mvwaddch(plane,event.y-3,event.x,ACS_CKBOARD); wrefresh(plane);


        for(int i=0;i<n;i++){
          if((nodes[i].x==event.x-xo) && (nodes[i].y==yo-event.y+3)){
            exist=true;
          }
        }
        if(exist==false){
        mvwaddch(plane,event.y-3,event.x,'O'); wrefresh(plane);
        nodes[n].x=event.x-xo;
        nodes[n].y=yo-event.y+3;
        nodes[n].active=true;
        n++;
      }
      }
    }
  }
}
void addlink(){
  float x,y;
  drawmenu(2,1);
  MEVENT event;
  mousemask(BUTTON1_CLICKED,NULL);
  bool loop=true;
  while(loop==true){
  int link1=-1,link2=-1;
  wclear(footer); mvwprintw(footer,0,0,"Add first link"); wrefresh(footer);
  if( wgetch(plane) == KEY_MOUSE )
  {
    getmouse(&event);
    if((event.y==1) && (event.x>width-10)){
      loop=false;
    }
    if(event.x>3 && event.x<width-20 && event.y>3 && event.y<width-1){
    x=event.x;
    y=event.y;
    for(int i=0;i<n;i++){
      if((nodes[i].x==x-xo) && (nodes[i].y==yo-y+3) ){
        link1=i;
        break;
      }

    }
  }
  }


  if(loop==true){
    mvwprintw(footer,0,0,"Add second link"); wrefresh(footer);
  if( wgetch(plane) == KEY_MOUSE )
  {
    getmouse(&event);
    if((event.y==1) && (event.x>width-10)){
      loop=false;
    }
    if(event.x>3 && event.x<width-20 && event.y>3 && event.y<width-1){
    x=event.x;
    y=event.y;
    for(int i=0;i<n;i++){
      if((nodes[i].x==x-xo) && (nodes[i].y==yo-y+3) && (link1!=i) ){
        link2=i;
        break;
      }
    }
  }
  }
}
  if(link1!=-1 && link2!=-1){
    links[m].one=link1;
    links[m].two=link2;
    m++;
    wprintw(list," %d - %d\n",link1,link2); wrefresh(list);
  }
}
}
void save(){
  char namefile[20];
  mvwaddstr(footer,0,0,"File name: ");
  echo();
  wgetstr(footer, namefile);
  noecho();
  ofstream mfile;
  mfile.open(namefile);
  mfile << n <<"\n";
  for(int i=0;i<n;i++){
    mfile << i << " " << nodes[i].x <<" "<< nodes[i].y <<"\n";
  }
  for(int i=0;i<m;i++){
    mfile << links[i].one <<" "<< links[i].two <<"\n";
  }
  mfile.close();
}
void drawmenu(int a, int b){
  wclear(menu);
  mvwprintw(menu ,1,1,"|Node");
  mvwprintw(menu,1,11,"|Link");
  mvwprintw(menu ,1,21,"|Save");
  wattron(menu,A_BOLD | A_UNDERLINE);
  switch(a){
    case 1:
      mvwprintw(menu ,1,1,"|Node");
      break;
    case 2:
      mvwprintw(menu ,1,11,"|Link");
      break;
    }
    wattroff(menu,A_BOLD | A_UNDERLINE);
    if(b==1){
      mvwprintw(menu,1,width-10,"| Exit");
    }
  box(menu,0,0);
  wrefresh(menu);

}
int main(int argc, char const *argv[]) {


  bool close;
  initscr();  getmaxyx(stdscr,height,width); keypad(stdscr,TRUE); noecho(); start_color();

  width--; height--;
  menu=newwin(3,width,0,0);
  keypad(menu, TRUE);
  plane=newwin(height-3,width-20,3,0);
  keypad(plane, TRUE);
  footer=newwin(1,width,height,0);
  list=newwin(height-3,20,3,width-19);
  box(plane,0,0);

  drawmenu(0,1);
  yo=(height-4)/2+3;
  xo=(width-20)/2;


  init_color(COLOR_RED, 140, 140, 140);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  //init_pair(2, COLOR_WHITE, COLOR_BLACK);
  wattron(plane, COLOR_PAIR(1));
  int b;
  for(int i=0;i<width-22;i++){
    for(b=0;b<height-5;b++){
    if((i+b)%2==0){mvwaddch(plane ,b+1,i+1,ACS_CKBOARD);}
  }
  }
  wattroff(plane, COLOR_PAIR(1));
  mvwprintw(plane ,yo,xo,"+");
  wrefresh(plane);
  wrefresh(menu);
  MEVENT event;
  mousemask(BUTTON1_CLICKED,NULL);
  while(close==false){
    //move(height-1,0);
    //clrtoeol();
    int ch= wgetch(menu);
    if( ch == KEY_MOUSE )
    {
      getmouse(&event);
      if(event.y==1){
        int x=event.x;
        if(x<10){
          addnode();
        }
        if(x>=11 && x<20){
          addlink();
        }
        if(x>=21 && x<30){
          save();
        }
        if((event.y==1) && (event.x>width-10)){
          endwin();
          return 0;
        }
      }

    }
    else if(ch == 'q'){close=true;}
    drawmenu(0,1);
    wclear(footer); wrefresh(footer);
  }
  endwin();

  return 0;
}

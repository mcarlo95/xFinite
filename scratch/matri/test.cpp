#include <math.h>
#include <iostream>
#include <fstream>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
using namespace std;

class Node
{
public:
  double x=0,y=0,xp=0,yp=0,xpp=0,ypp=0;
};

class Force{
public:
  double x=0,y=0;
};

class Link
{
public:
  double c=0,s=0,l=0,l0=0;
  int one=0,two=0;
  void refresh(double x1,double y1,double x2,double y2){
    l=hypot(x2-x1,y2-y1);
    c=(x2-x1)/l;
    s=(y2-y1)/l;
  }
  void lenght0(double x1,double y1,double x2,double y2){
    l0=hypot(x2-x1,y2-y1);
  }
};

int main(int argc, char const *argv[]) {

  double ke=100000;
  double m=25;
  double dt=0.01;
  double smorzamento=0.002;

  int row, col;
  int nN,nL=0; //numero di nodi,links
  int k,i,j,b,a; //contatori
  int one,two;

  //inizializzazione ncurses
  initscr();
  raw();
  getmaxyx(stdscr,row,col);
  noecho();
  nodelay(stdscr,TRUE);
  keypad(stdscr,TRUE);
  start_color();
  init_pair(1, COLOR_WHITE, COLOR_BLACK);
  //bkgd(COLOR_PAIR(1));
  attron(COLOR_PAIR(1));

  float ampl=1;

  //ofstream mfile;
  //mfile.open("log.txt");

  i=1;
  string filename;
  string config;
  while(i<argc){
    if (string(argv[i])=="-c") {
      config = argv[i+1];
      i=i+2;
    } else if (string(argv[i])=="-h") {
      cout << "Finite to simulate phisics structures\n\nUsage:\n ./finite <ship.ship> -c <config>\n";
return 0;
    } else {
      filename = argv[i];
      i++;
    }

  }

  //mfile << argv[1] <<filename << config << ampl;
  ifstream file;
  file.open(filename); if(!file.is_open()){ cout << "errore apertura file\n "; endwin(); return 1;}

  file >> nN;
  Node nodes[nN];
  Link links[4*nN];
  Force forces[nN];
  double matrix[2*(nN)][2*(nN)];

  for(i=0;i<nN;i++){
    file >> k;
    file >> nodes[k].x >> nodes[k].y;
  }
  /*
  for(i=0;i<nN;i++){ cout << nodes[i].x << " " << nodes[i].y << "\n"; }
  */
  while ( !file.eof() ){
    file >> links[nL].one >> links[nL].two;
    nL++;
  }
  nL=nL-1;;

  file.close();
  file.open(config);
  if(!file.is_open()){ cout << "nessuna impostazione\n";}
  if(file.is_open()){ file >> dt >> m >> ke >> ampl >> smorzamento; }
  file.close();

  //cout << "nodi:" << nN << " connessioni:" << nL<<"\n";

  for(i=0;i<nL;i++){ //calcolo l0
    one=links[i].one;
    two=links[i].two;
    links[i].lenght0(nodes[one].x,nodes[one].y,nodes[two].x,nodes[two].y);
  }

  double mk[2*(nN+1)][nL];

  //ofstream mfile;
  //mfile.open("log.txt");

  while(a>=0){ // inizio del ciclo!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
    a++;

    for(i=0;i<nL;i++){ //calcolo l,c,s
      one=links[i].one; two=links[i].two;
      links[i].refresh(nodes[one].x,nodes[one].y,nodes[two].x,nodes[two].y);
    }

    /* graphics */
    if(b>1.0/dt/50){
      clear();
      for(i=0;i<nN;i++){
        move((nodes[i].y)*ampl+row/2+0.5,(nodes[i].x)*ampl*2+col/2+0.5);
        addch(ACS_DIAMOND);
      }
      move(row-1,1); printw("%f",a*dt);
      refresh();
      //for(j=0;j<nN;j++){ mfile << nodes[j].xpp << ":" << nodes[j].ypp << " ";} mfile << "\n";
      b=0;

    }
    b++;
    switch(getch())
    {	case KEY_LEFT:
      forces[0].x=1000*links[0].c;
      forces[0].y=1000*links[0].s;
      break;
      case KEY_RIGHT:
      forces[1].x=1000*links[1].c;
      forces[1].y=1000*links[1].s;
      break;
      case KEY_UP:
      forces[2].x=3000*links[2].c;
      forces[2].y=3000*links[2].s;
      break;
      case KEY_DOWN:
      forces[2].x=-500*links[2].c;
      forces[2].y=-500*links[2].s;
      break;
      case 'q':
      a=-10;
      break;
      default:
      forces[0].x=0;
      forces[0].y=0;
      forces[1].x=0;
      forces[1].y=0;
      forces[2].x=0;
      forces[2].y=0;
      break;
    }


    usleep(500000*dt);


    for(i=0;i<2*(nN);i++){
      for(j=0;j<2*(nN);j++){
        matrix[i][j]=0;
      }
    }
    for(i=0;i<nL;i++){
      one=links[i].one; two=links[i].two;
      matrix[2*one][2*one]+=-pow(links[i].c,2)/links[i].l0;
      matrix[2*one][2*one+1]+=-links[i].c*links[i].s/links[i].l0;
      matrix[2*one+1][2*one+1]+=-pow(links[i].s,2)/links[i].l0;
      matrix[2*one][2*two]+=pow(links[i].c,2)/links[i].l0;
      matrix[2*one][2*two+1]+=links[i].c*links[i].s/links[i].l0;
      matrix[2*one+1][2*two]+=links[i].c*links[i].s/links[i].l0;
      matrix[2*one+1][2*two+1]+=pow(links[i].s,2)/links[i].l0;
      matrix[2*two][2*two]+=-pow(links[i].c,2)/links[i].l0;
      matrix[2*two][2*two+1]+=-links[i].c*links[i].s/links[i].l0;
      matrix[2*two+1][2*two+1]+=-pow(links[i].s,2)/links[i].l0;
      matrix[2*one+1][2*one]=matrix[2*one][2*one+1];
      matrix[2*two][2*one]=matrix[2*one][2*two];
      matrix[2*two+1][2*one]=matrix[2*one][2*two+1];
      matrix[2*two][2*one+1]=matrix[2*one+1][2*two];
      matrix[2*two+1][2*one+1]=matrix[2*one+1][2*two+1];
      matrix[2*two+1][2*two]=matrix[2*two][2*two+1];
    }
    //for(i=0;i<2*(nN);i++){ mfile << matrix[2][i] << " "; }

    //crea la matrice degli allungamenti iniziali l0
    for(i=0;i<2*(nN);i++){
      for(j=0;j<nL;j++){
        mk[i][j]=0;
      }
    }
    for(i=0;i<nL;i++){
      one=links[i].one; two=links[i].two;
      mk[2*one][i]=-links[i].c;
      mk[2*one+1][i]=-links[i].s;
      mk[2*two][i]=links[i].c;
      mk[2*two+1][i]=links[i].s;
    }
    //for(i=0;i<nL;i++){ mfile << mk[2][i] << " "; } mfile << "\n";

    //calcola le accelerazioni dei punti
    for(i=0;i<nN;i++){

      nodes[i].xpp=0;
      for(j=0;j<nN;j++){
        nodes[i].xpp+=matrix[2*i][2*j]*nodes[j].x;
        nodes[i].xpp+=matrix[2*i][2*j+1]*nodes[j].y;
      }
      for(j=0;j<nL;j++){
        nodes[i].xpp+=mk[2*i][j];
      }
      nodes[i].xpp=(ke*nodes[i].xpp+forces[i].x)/m; //potenza

      nodes[i].ypp=0;
      for(j=0;j<nN;j++){
        nodes[i].ypp+=matrix[2*i+1][2*j]*nodes[j].x;
        nodes[i].ypp+=matrix[2*i+1][2*j+1]*nodes[j].y;
      }
      for(j=0;j<nL;j++){
        nodes[i].ypp+=mk[2*i+1][j];
      }
      nodes[i].ypp=(ke*nodes[i].ypp+forces[i].y)/m;

    }
    //calcola velocità e spostamenti*/
    for(i=0;i<nN;i++){
      nodes[i].xp+=dt*nodes[i].xpp-nodes[i].xp*smorzamento;
      nodes[i].x+=dt*nodes[i].xp;

      nodes[i].yp+=dt*nodes[i].ypp-nodes[i].yp*smorzamento;
      nodes[i].y+=dt*nodes[i].yp;
    }
  }
  endwin();
  return 0;
}

#include <iostream>
#include <cstdlib>
#include <conio.h>

#include "windows.h"

using namespace std;

bool gameOver;

const int width=20;
const int height=20;
int x,y;//head position
int fruitX,fruitY,score;
enum eDirection{STOP=0,LEFT,RIGHT,UP,DOWN};//snake direction controls
eDirection dir;
int tailX[100], tailY[100];
int nTail;


void Setup()
{
    gameOver=false;
    dir=STOP;
    x=width/2;//place head in mid of screen
    y=height/2;//place head in mid of screen
    fruitX=rand()%width;
    fruitY=rand()%height;
    score=0;

}

void Draw()
{
    system("cls");//if not work "clear"
    for(int i=0;i<width+2;i++)//did +2 to complete the rectangle
    {
        cout<<"U";
    }
    cout<<endl;
   for(int i=0;i<height;i++)
   {

       for(int j=0;j<width;j++)
       {
            if(j==0)
            {
                cout<<"L";
            }

            if(i==y&&j==x)
                cout<<"O";
            else if((i==fruitY)&&(j==fruitX))
                cout<<"F";
            else
            {
                bool print=false;
             for(int k=0;k<nTail;k++)
             {

                 if(tailX[k]==j&& tailY[k]==i)
                 {
                     cout<<"o";
                     print=true;
                 }
            }
            if(!print)
                cout<<" ";
        }

            if(j==width-1)
                cout<<"R";
       }
       cout<<endl;
   }
    for(int i=0;i<width+2;i++)//did +2 to complete the rectangle
    {
        cout<<"D";
    }
    cout<<endl;
    cout<<"Score: "<<score<<endl;

}

void Input()
{
    if(_kbhit())
    {
        switch(_getch())
        {

        case 'a':
            dir=LEFT;
            break;
        case 'd':
            dir=RIGHT;
            break;
        case 'w':
            dir=UP;
            break;
        case 's':
            dir=DOWN;
            break;
        case 'x':
            gameOver=true;
            break;
        }
    }

}

void Logic()
{
    int prevX=tailX[0];
    int prevY=tailY[0];
    int prev2X,prev2Y;
    tailX[0]=x;
    tailY[0]=y;
    for(int i=1;i<nTail;i++)
    {
        prev2X=tailX[i];
        prev2Y=tailY[i];
        tailX[i]=prevX;
        tailY[i]=prevY;
        prevX=prev2X;
        prevY=prev2Y;
    }

    switch(dir)
    {
    case LEFT:
        x--;
        break;
    case RIGHT:
        x++;
        break;
    case UP:
        y--;
        break;
    case DOWN:
        y++;
        break;

    default:
        break;
    }

    if(x>width||x<0||y>height||y<0)
        gameOver=true;
    else if((x==fruitX)&&(y=fruitY))
    {
        score+=10;
        fruitX=rand()%width;
        fruitY=rand()%height;
        nTail++;
    }

}
int main()
{
    Setup();

    while(!gameOver)
    {
        Draw();
        Input();
        Logic();
        Sleep(100);
    }


    return 0;
}

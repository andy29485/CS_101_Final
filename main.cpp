#include <iostream>
#include <cstdlib>
#include "windows.h"
#include <conio.h>

/*#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"

#define TFT_DC 9
#define TFT_CS 10
*/

using namespace std;


bool gameOver;//if true then the game ends, if false then the game continues

const int width=30;//width of the screen
const int height=20;//height of the screen
int score;
int birdX,birdY;//The Bird's X and Y position
int pipesOneX,pipesOneY,pipesTwoX,pipesTwoY;//the first 2 pipes' X and Y positions
int pipes3X,pipes3Y,pipes4X,pipes4Y;//the last 2 pipes' X and Y positions
enum pipeDir{LEFT};//Makes the pipes constantly move left
enum birdDir{UP,DOWN};//moves the direction down when no button pressed and up when 'w' pressed
pipeDir pipe;
birdDir bird;

void Setup()
{
    score=0;
    gameOver=false;
    //initial X and Y positions set for bird and all 4 pipes
    birdX=3;
    birdY=9;

    pipesOneX=10;
    pipesOneY=(rand()%6)+5;//it's supposed to give random numbers 5-10, This might not do this

    pipesTwoX=10;
    pipesTwoY=pipesOneY+7;


    pipes3X=20;
    pipes3Y=(rand()%6)+5;

    pipes4X=20;
    pipes4Y=pipes3Y+7;

}

void Draw()
{

    system("cls");

    //prints the screen with the pipes '|' and the bird 'O'
    for(int i=0;i<width+1;i++)//did +2 to complete the rectangle
        cout<<"_";
    cout<<endl;

   for(int i=0;i<height;i++)
   {

       for(int j=0;j<width;j++)
       {
            if(j==0)
                cout<<"|";
            else if((i==birdY)&&(j==birdX))//prints the bird
                cout<<"O";
            else if(j==pipesOneX&&i<=pipesOneY)
                {
                    cout<<"|";

                }
            else if(j==pipesTwoX&&i>=pipesTwoY)
            {
                cout<<"|";

            }

            else if(j==pipes3X&&i<=pipes3Y)
            {

                cout<<"|";
            }
            else if(j==pipes4X&&i>=pipes4Y)
                cout<<"|";

           else
                cout<<" ";


            if(j==width-1)
                cout<<"|";
       }
       cout<<endl;
    }
    for(int i=0;i<width+1;i++)//did +2 to complete the rectangle
    {
        cout<<"_";
    }

    cout<<endl;


    cout<<"Score: "<<score<<endl;
}

void Input()//needs to be changed so it moves up when you tap the screen
{

    if(_kbhit())//if the 'w' key is pressed then it goes up
    {
        switch(_getch())
        {
        case 'w':
            bird=UP;
            break;
        }
    }
    else//otherwise the bird goes down
        bird=DOWN;

}

void Logic()
{


    switch(bird)
    {

    case UP:
        birdY=birdY-4;//makes bird jump up 4
        break;
    case DOWN:
        birdY++;//makes bird move down
    }

    switch(pipe)
    {
    case LEFT:
        //moves all the pipes left
        pipesOneX--;
        pipesTwoX--;
        pipes3X--;
        pipes4X--;
        break;
    }
    if(birdY>height||birdY<0)//ends game if bird flies hits the ceiling and if bird hits the ground, not sure if this is a rule
                            //or not
        gameOver=true;

    if(((birdX==pipesOneX)&&(birdY<=pipesOneY))||((birdX==pipesTwoX)&&(birdY>=pipesTwoY))||((birdX==pipes3X)&&(birdY<=pipes3Y))||((birdX==pipes4X)&&(birdY>=pipes4Y)))//if bird touches a wall, ends game
        gameOver=true;

    if(pipesOneX==0)//when the wall reaches the end of the screen, this creates a new wall
    {
        pipesOneX=29;
        pipesOneY=(rand()%6)+5;
    }

    if(pipesTwoX==0)// when this wall reaches the end of the screen. this creates a new wall
    {
        pipesTwoX=29;
        pipesTwoY=pipesOneY+7;
    }

    if(pipes3X==0)// when this wall reaches the end, a new wall is created
    {
        pipes3X=29;
        pipes3Y=(rand()%6)+5;
    }

    if(pipes4X==0)//when this wall reaches the end, a new wall is created
    {
        pipes4X=29;
        pipes4Y=pipes3Y+7;
    }

    score++;//score is incremented
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


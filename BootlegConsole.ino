#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"

#define TFT_DC 9
#define TFT_CS 10

#define SNAKE_SIZE     18
#define SNAKE_POINT    -1
#define SNAKE_WALL     -2
#define DELAY_TIME    400
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 320

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

using namespace std;

const int screensizeX = 240;
const int screensizeY = 320;
const int shift = 5;

const int sizeX = 20;
const int sizeY = 20;

const int ratio = screensizeX / sizeX;

int lifepoints = 4;
int enemyCount = 15;
int gamespeed = 100;
int shootingrate = 3;

char spaceship = '^';
char enemy = 'O';
char spaceshipMissile = ':';
char enemyMissile = '-';
char border = '.';
char space = ' ';

bool endgame = false;
bool gameover = false;

String life = "Life Points: ";
String lifesymbol = "* ";
String win = "Congratulations!";
String lose = "Game Over!";

char GameSpace[20][20] = {
  "...................",
  ".                 .",
  ". O   O    O    O .",
  ".   O   O    O    .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".                 .",
  ".              ^  .",
  "..................."
};

enum Direction {
  LEFT,
  UP,
  RIGHT,
  DOWN
};

typedef struct {
  int       x;
  int       y;
  int       score;
  Direction dir;
} Player;

void setup() {
  tft.begin();
  ts.begin();

  // Splash screen
  unsigned long start, t;
  int x1, y1, x2, y2, w = tft.width(), h = tft.height();

  tft.fillScreen(ILI9341_BLACK);
  yield();
  x1 = y1 = 0;
  y2 = h - 1;
  start = micros();
  for(x2=0; x2<w; x2+=6) tft.drawLine(x1, y1, x2, y2, ILI9341_RED);
  x2 = w - 1;
  for(y2=0; y2<h; y2+=6) tft.drawLine(x1, y1, x2, y2, ILI9341_RED);
  t = micros() - start;
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextSize(3);
  tft.setCursor(10,80);
  tft.print("Bootleg");
  tft.setCursor(10,110);
  tft.print("Console");
  tft.setCursor(10,140);
  tft.setTextSize(2);
  tft.print("Version 1.0");
  delay(2000);
  
}

void loop() {
    int touch = 0, choice = 0;
    drawMenu();

    while (touch == 0) {
      if (ts.touched()) {
        TS_Point p = ts.getPoint();
        p.x = map(p.x, 0, 240, 240, 0);
        p.y = map(p.y, 0, 320, 320, 0);

        if (p.x >= 30 && p.x < 210 && p.y >= 80 && p.y < 130)
          choice = 0;
        else if (p.x >= 30 && p.x < 210 && p.y >= 140 && p.y < 190)
          choice = 1;
        else if (p.x >= 30 && p.x < 210 && p.y >= 200 && p.y < 250)
          choice = 2;
        else if (p.x >= 30 && p.x < 210 && p.y >= 260 && p.y < 310)
          choice = 3;

        touch = 1;
        delay(500);
      }
    }

    switch (choice) {
      case 0: TicTacToe(); break;
      case 1: Snake(); break;
      case 2: FlappyBird(); break;
      case 3: SpaceInvaders(); break;
    }
}

void drawMenu() {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.setCursor(25,30);
      tft.print("Select a game");

      tft.setTextColor(ILI9341_BLACK);
      tft.fillRect(25, 80, 190, 50, ILI9341_WHITE);
      tft.setCursor(55,100);
      tft.print("Tic Tac Toe");
      tft.fillRect(25, 140, 190, 50, ILI9341_WHITE);
      tft.setCursor(85,160);
      tft.print("Snake");
      tft.fillRect(25, 200, 190, 50, ILI9341_WHITE);
      tft.setCursor(55,220);
      tft.print("Flappy Box");
      tft.fillRect(25, 260, 190, 50, ILI9341_WHITE);
      tft.setCursor(40,280);
      tft.print("Space Invaders");
}

void TicTacToe() {
  int board[9] = { 0 };
  int i, position, turn, winner, touch = 0;
  boolean compPlayer;

      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.setCursor(25,30);
      tft.print("Choose mode:");
      tft.setTextColor(ILI9341_BLACK);
      tft.fillRect(25, 140, 190, 50, ILI9341_WHITE);
      tft.setCursor(45,160);
      tft.print("vs. AI (Easy)");
      tft.fillRect(25, 200, 190, 50, ILI9341_WHITE);
      tft.setCursor(75,220);
      tft.print("2 Player");

  while (touch == 0) {
      if (ts.touched()) {
        TS_Point p = ts.getPoint();
        p.x = map(p.x, 0, 240, 240, 0);
        p.y = map(p.y, 0, 320, 320, 0);

        if (p.x >= 30 && p.x < 210 && p.y >= 140 && p.y < 190)
          compPlayer = true;
        else if (p.x >= 30 && p.x < 210 && p.y >= 200 && p.y < 250)
          compPlayer = false;

        touch = 1;
        delay(500);
      }
    }
  drawBoard();
  turn = 1;

  for (i = 0; i < 9; i++) {
    if (turn == 1) {
        tft.fillRect(25, 270, 110, 20, ILI9341_BLACK);
        tft.setCursor(30,280);
        tft.setTextSize(1);
        tft.print("Player 1's turn");

        do {
        position = getInput();
        } while (board[position] != 0);
        board[position] = 1;
        drawPlayer1(position);
        delay(500);
    }
    else if (turn == 2) {
        tft.fillRect(25, 270, 110, 15, ILI9341_BLACK);
        tft.setCursor(30,280);
        tft.setTextSize(1);
        tft.print("Player 2's turn");

        do {
          if (compPlayer) {
            position = random() % 9;
            delay(250);
          }
          else {
              position = getInput();
          }
        } while (board[position] != 0);
        board[position] = 2;
        drawPlayer2(position);
        delay(500);
    }

    winner = winConditions(board);

    if (winner == 1) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("Player 1 Wins!");
        delay(2000);
        break;
    }
    else if (winner == 2) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("Player 2 Wins!");
        delay(2000);
        break;
    }

    if (turn == 1)
      turn = 2;
    else
      turn = 1;
  }
    if (winner == 0) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("It's a tie!");
        delay(2000);
    }
}

int getInput() {
  int touch = 0, position;

  while (touch == 0) {
    if (ts.touched()) {
      TS_Point p = ts.getPoint();
      p.x = map(p.x, 0, 240, 240, 0);
      p.y = map(p.y, 0, 320, 320, 0);

      if (p.x >= 30 && p.x < 90 && p.y >= 60 && p.y < 120)
        position = 0;
      else if (p.x >= 90 && p.x < 150 && p.y >= 60 && p.y < 120)
        position = 1;
      else if (p.x >= 150 && p.x < 210 && p.y >= 60 && p.y < 120)
        position = 2;
      else if (p.x >= 30 && p.x < 90 && p.y >= 120 && p.y < 180)
        position = 3;
      else if (p.x >= 90 && p.x < 150 && p.y >= 120 && p.y < 180)
        position = 4;
      else if (p.x >= 150 && p.x < 210 && p.y >= 120 && p.y < 180)
        position = 5;
      else if (p.x >= 30 && p.x < 90 && p.y >= 180 && p.y < 240)
        position = 6;
      else if (p.x >= 90 && p.x < 150 && p.y >= 180 && p.y < 240)
        position = 7;
      else if (p.x >= 150 && p.x < 210 && p.y >= 180 && p.y < 240)
        position = 8;

      touch = 1;
    }
  }
  return position;
}

void drawBoard() {
  tft.fillScreen(ILI9341_BLACK);

  tft.setCursor(60,20);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Tic Tac Toe");

  tft.drawLine(90,60,90,240,ILI9341_WHITE);
  tft.drawLine(150,60,150,240,ILI9341_WHITE);
  tft.drawLine(30,120,210,120,ILI9341_WHITE);
  tft.drawLine(30,180,210,180,ILI9341_WHITE);
}

void drawX(int x, int y) {
  tft.drawLine(x-15, y-15, x+15, y+15,ILI9341_RED);
  tft.drawLine(x+15, y-15, x-15, y+15,ILI9341_RED);
}

void drawPlayer1(int position) {
    switch(position) {
    case 0: tft.drawCircle(60,85,20,ILI9341_BLUE);  break;
    case 1: tft.drawCircle(120,85,20,ILI9341_BLUE); break;
    case 2: tft.drawCircle(180,85,20,ILI9341_BLUE); break;
    case 3: tft.drawCircle(60,150,20,ILI9341_BLUE);  break;
    case 4: tft.drawCircle(120,150,20,ILI9341_BLUE); break;
    case 5: tft.drawCircle(180,150,20,ILI9341_BLUE); break;
    case 6: tft.drawCircle(60,210,20,ILI9341_BLUE); break;
    case 7: tft.drawCircle(120,210,20,ILI9341_BLUE); break;
    case 8: tft.drawCircle(180,210,20,ILI9341_BLUE); break;
  }
}

void drawPlayer2(int position) {
    switch(position) {
    case 0: drawX(60,85);  break;
    case 1: drawX(120,85); break;
    case 2: drawX(180,85); break;
    case 3: drawX(60,150);  break;
    case 4: drawX(120,150); break;
    case 5: drawX(180,150); break;
    case 6: drawX(60,210); break;
    case 7: drawX(120,210); break;
    case 8: drawX(180,210); break;
  }
}

int winConditions(int board[]) {
  int winner = 0;

  if (board[0] == 1 && board[1] == 1 && board[2] == 1)
    winner = 1;
  if (board[3] == 1 && board[4] == 1 && board[5] == 1)
    winner = 1;
  if (board[6] == 1 && board[7] == 1 && board[8] == 1)
    winner = 1;
  if (board[0] == 1 && board[3] == 1 && board[6] == 1)
    winner = 1;
  if (board[1] == 1 && board[4] == 1 && board[7] == 1)
    winner = 1;
  if (board[2] == 1 && board[5] == 1 && board[8] == 1)
    winner = 1;
  if (board[0] == 1 && board[4] == 1 && board[8] == 1)
    winner = 1;
  if (board[2] == 1 && board[4] == 1 && board[6] == 1)
    winner = 1;

  if (board[0] == 2 && board[1] == 2 && board[2] == 2)
    winner = 2;
  if (board[3] == 2 && board[4] == 2 && board[5] == 2)
    winner = 2;
  if (board[6] == 2 && board[7] == 2 && board[8] == 2)
    winner = 2;
  if (board[0] == 2 && board[3] == 2 && board[6] == 2)
    winner = 2;
  if (board[1] == 2 && board[4] == 2 && board[7] == 2)
    winner = 2;
  if (board[2] == 2 && board[5] == 2 && board[8] == 2)
    winner = 2;
  if (board[0] == 2 && board[4] == 2 && board[8] == 2)
    winner = 2;
  if (board[2] == 2 && board[4] == 2 && board[6] == 2)
    winner = 2;

   return winner;
}

// SNAKE

void Snake() {
  // create map and player
  char snake_map[SNAKE_SIZE][SNAKE_SIZE];
  Player p;

  // put walls around the border
  for(int j=0; j<SNAKE_SIZE; ++j) {
    for(int i=0; i<SNAKE_SIZE; ++i) {
      snake_map[j][i] = 0;
    }
  }
  for(int i=0; i<SNAKE_SIZE; ++i) {
    snake_map[           0][           i] = SNAKE_WALL;
    snake_map[SNAKE_SIZE-1][           i] = SNAKE_WALL;
    snake_map[           i][SNAKE_SIZE-1] = SNAKE_WALL;
    snake_map[           i][           0] = SNAKE_WALL;
  }

  // put the player on the map
  p.x     = SNAKE_SIZE/2;
  p.y     = SNAKE_SIZE/2;
  p.score = 0;
  snake_map[p.y][p.x] = 1;

  // start loop
  snake_main_loop(snake_map, p);
}

void snake_main_loop(char (&snake_map)[SNAKE_SIZE][SNAKE_SIZE], Player& p) {
  snake_display(snake_map, p.score);
  unsigned long curMillis = millis();
  unsigned long oldMillis = millis();
  do {
    curMillis = millis();
    snake_process_input(p);
    if(curMillis-oldMillis > DELAY_TIME) {
      oldMillis = curMillis;
      if(!snake_move(snake_map, p))
        break;
    }
  } while(true);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(30,150);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Game over!");
  delay(2000);
}

// displays the map
//   returns false on game over
void snake_display(char (&snake_map)[SNAKE_SIZE][SNAKE_SIZE], const int& score) {
  boolean x_longer = SCREEN_SIZE_X > SCREEN_SIZE_Y;
  int     width    = (x_longer ? SCREEN_SIZE_Y : SCREEN_SIZE_X)/SNAKE_SIZE;
  int     offset_x =  x_longer ? SCREEN_SIZE_X-SCREEN_SIZE_Y : 0;
  int     offset_y = !x_longer ? SCREEN_SIZE_Y-SCREEN_SIZE_X : 0;
  int     colour   = ILI9341_GREEN;

  tft.fillScreen(ILI9341_BLACK);  // TODO - not sure about the arduino API
  tft.setCursor(10, 5);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.print("Score:  ");
  tft.print(score);
  //tft.printNumI(score, 4, 4);   // TODO - not sure about the arduino API

  for(int i=0; i<SNAKE_SIZE; ++i) {
    for(int j=0; j<SNAKE_SIZE; ++j) {
      // point
      if(snake_map[j][i] == SNAKE_POINT)
        colour = ILI9341_RED;

      // wall
      else if(snake_map[j][i] == SNAKE_WALL)
        colour = ILI9341_BLUE;

      // player
      else if(snake_map[j][i] > 0)
        colour = ILI9341_GREEN;

      // nothing
      else
        colour = 0;

      if(colour) // TODO maybe this needs to be `if(colour != 0)`
        tft.fillRect(i*width+offset_x/2,j*width+offset_y/2,width,width,colour);
    }
  }
}

bool snake_move(char (&snake_map)[SNAKE_SIZE][SNAKE_SIZE], Player& p) {
  int nPoints  = 0;
  int x, y;

  // move to next space
  switch(p.dir) {
    case LEFT:
      ++p.x;
      break;
    case RIGHT:
      --p.x;
      break;
    case UP:
      --p.y;
      break;
    case DOWN:
      ++p.y;
      break;
  }

  // perform checks for special conditions
  if(snake_map[p.y][p.x] == SNAKE_POINT) {   // point get
    ++p.score;
  }
  else if(snake_map[p.y][p.x] != 0) {  // crash
    return false;
  }

  boolean x_longer = SCREEN_SIZE_X > SCREEN_SIZE_Y;
  int     width    = (x_longer ? SCREEN_SIZE_Y : SCREEN_SIZE_X)/SNAKE_SIZE;
  int     offset_x =  x_longer ? SCREEN_SIZE_X-SCREEN_SIZE_Y : 0;
  int     offset_y = !x_longer ? SCREEN_SIZE_Y-SCREEN_SIZE_X : 0;
  int     colour   = ILI9341_BLACK;

  // update the map - "move the player"
  for(int i=0; i<SNAKE_SIZE; ++i) {
    for(int j=0; j<SNAKE_SIZE; ++j) {
      if(snake_map[j][i] > 0)
        ++snake_map[j][i];
      else if(snake_map[j][i] == SNAKE_POINT)
        ++nPoints;
      if(snake_map[j][i] > p.score+4) {
        tft.fillRect(i*width+offset_x/2,j*width+offset_y/2,width,width,colour);
        snake_map[j][i] = 0;
      }
    }
  }

  colour = ILI9341_BLACK;
  tft.fillRect(0, 0, width*SNAKE_SIZE, width, colour);
  tft.setCursor(10, 5);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Score:  ");
  tft.print(p.score);

  // Put poins on the field if none are there
  while(!nPoints) {
    x = random() % (SNAKE_SIZE-2) + 1; // account for outer walls
    y = random() % (SNAKE_SIZE-2) + 1;
    if (snake_map[y][x] == 0) {
      snake_map[y][x] = SNAKE_POINT;
      colour = ILI9341_RED;
      tft.fillRect(x*width+offset_x/2,y*width+offset_y/2,width,width,colour);
      ++nPoints;
    }
  }

  // set the head of the player to their current location
  snake_map[p.y][p.x] = 1;
  colour = ILI9341_GREEN;
  tft.fillRect(p.x*width+offset_x/2, p.y*width+offset_y/2, width,width, colour);
  return true;
}

void snake_process_input(Player& player) {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);

    // split the screen up into an x and check for inputs on each size of it

    if(p.y <= 90) {
      player.dir = DOWN;
    }
    if (p.y >= 230) {
      player.dir = UP;
    }
    if (p.y > 50 && p.y < 270 && p.x > 0 && p.x <= 120) {
      player.dir = LEFT;
    }
    if(p.y > 50 && p.y < 270 && p.x > 120 && p.x <= 240) {
      player.dir = RIGHT;
    }
  }
}

// FLAPPY BIRD

void FlappyBird() {
      boolean play = true;
      int x = 240, birdX = 50, birdY=180, score = 0;

      int y = random(80,200);
      tft.fillScreen(ILI9341_BLACK);
      while (play) {
        drawPipes(x, y);
        x-=1;

        tft.fillRect(birdX, birdY, 20, 20, ILI9341_RED);
        tft.fillRect(birdX, birdY-20, 20, 20, ILI9341_BLACK);
        birdY+=1;
        if (ts.touched()) {
          birdY -= 20;
          tft.fillRect(birdX, birdY+20, 20, 20, ILI9341_BLACK);
          delay(10);
        }
        if ((birdX == x || birdX + 20 == x) && (birdY < y || birdY + 20 > (60 + y))) {
            FlappyBirdGameOver(score);
            break;
        }
        if (birdY < 0 || birdY + 20 > 320) {
            FlappyBirdGameOver(score);
            break;
        }
        if (x < -1) {
          x = 240;
          y = random(80,200);
          drawPipes(x, y);
          score++;
        }
        delay(10);
      }
}

void FlappyBirdGameOver(int score) {
            tft.fillScreen(ILI9341_BLACK);
            tft.setCursor(30,150);
            tft.setTextColor(ILI9341_WHITE);
            tft.setTextSize(2);
            tft.print("Game over!");
            tft.setCursor(30,180);
            tft.print("Score:");
            tft.setCursor(120,180);
            tft.print(score);
            delay(2000);
}
void drawPipes(int x, int y) {
     tft.drawLine(x, 0, x, y, ILI9341_WHITE);
     tft.drawLine(x+1, 0, x+1, y, ILI9341_BLACK);
     tft.drawLine(x, 320, x, 60+y, ILI9341_WHITE);
     tft.drawLine(x+1, 320, x+1, 60+y, ILI9341_BLACK);
}

// SPACE INVADERS

void convertToPixel(int x, int y, int * pixX, int * pixY) {

  *pixX = x * ratio;
  *pixY = y * ratio;
}

//void updateGameSpace(char array[sizeX][sizeY]) {
void updateGameSpace(char array[sizeX][sizeY], int i, int j){

  int pixelX;
  int pixelY;

  //for (int i = 0; i < sizeX; i++) {
    //for (int j = 0; j < sizeY; j++) {

      //convertToPixel(i, j, &pixelX, &pixelY);

      pixelX = j * ratio;
      pixelY = i * ratio;
      
      if(array[i][j] == space)
        tft.fillRect(pixelX, pixelY, ratio, ratio, ILI9341_WHITE);
      else if (array[i][j] == border)
        tft.fillRect(pixelX, pixelY, ratio, ratio, ILI9341_GREEN);
      else if (array[i][j] == enemy)
        tft.fillRect(pixelX, pixelY, ratio, ratio, ILI9341_BLACK);
      else if (array[i][j] == spaceship)
        tft.fillRect(pixelX, pixelY, ratio, ratio, ILI9341_BLUE);
      else if (array[i][j] == spaceshipMissile)
        tft.fillRect(pixelX, pixelY, shift, shift, ILI9341_ORANGE);
      else if(array[i][j] = enemyMissile)
        tft.fillRect(pixelX, pixelY, shift, shift, ILI9341_ORANGE);
   //  }
 // }

    String lifepts =  "Life Points ";
    for(int i = 0; i < lifepoints; i++)
        lifepts += "* ";
    tft.setCursor(20, 220);
    tft.print(lifepts);
}

void SpaceInvaders() {
  int position;
  tft.fillScreen(ILI9341_BLACK);

  for(int posY = 0; posY < sizeY; posY++){
    for(int posX = 0; posX < sizeX; posX++){
      updateGameSpace(GameSpace, posY, posX);
    }
  }
  
  while(endgame == false) {
    // Iterate through each game state position and do updates
    for(int posY = 0; posY < sizeY; posY++) {
      for(int posX = 0; posX < sizeX; posX++){

        char current = GameSpace[posY][posX];
            if (ts.touched()) {
                TS_Point p = ts.getPoint();
                p.x = map(p.x, 0, 240, 240, 0);
                p.y = map(p.y, 0, 320, 320, 0);

                if (p.x >= 0 && p.x < 120 && p.y >= 260 && p.y < 320)
                  position = 1;
                else if (p.x >= 120 && p.x < 240 && p.y >= 260 && p.y < 320)
                  position = 2;
                else if (p.x >= 0 && p.x < 240 && p.y >= 0 && p.y < 260)
                  position = 3;
                else 
                  position = 0;
            }
        
        // Current position is where the spaceship is
        if (current == spaceship) {

          // User pressed Left Key, move spaceship one position left if it isnt against the corner
          if (position == 1) {

            if (posX > 1) {
              GameSpace[posY][posX] = space;
              updateGameSpace(GameSpace, posY, posX);
              
              posX--;
              
              GameSpace[posY][posX] = spaceship;
              updateGameSpace(GameSpace, posY, posX);
            }

          }

          // User pressed Right Key, move spaceship one position right if it isnt against the corner
          else if (position == 2) {

            if (posX < sizeX - 3) {
              GameSpace[posY][posX] = space;
              updateGameSpace(GameSpace, posY, posX);
              
              posX++;
              
              GameSpace[posY][posX] = spaceship;
              updateGameSpace(GameSpace, posY, posX);
            }
          }

          // User pressed Space Key, create missile one position up from spaceship.
          if (position == 3) {

            posY--;
            GameSpace[posY][posX] = spaceshipMissile;
            updateGameSpace(GameSpace, posY, posX);
          }

        }
       
        // Current position is where an enemy is. Randomize number to check if that enemy is firing a missile.
        else if (current == enemy) {

          if (rand() % 100 < shootingrate) {
            posY++;
            
            GameSpace[posY][posX] = enemyMissile;
            updateGameSpace(GameSpace, posY, posX);
          }
        }

        // Current position is where spaceship missile is.
        else if (current == spaceshipMissile) {

          // If missile isnt against top corner, it either moves up one or it hits an enemy
          if (posY > 1) {

            // Erase current position of missile
            GameSpace[posY][posX] = space;
            updateGameSpace(GameSpace, posY, posX);
            
            posY--;
            
            // If next position up is empty space put the missile there
            if(GameSpace[posY][posX] == space){
              GameSpace[posY][posX] = spaceshipMissile;
              updateGameSpace(GameSpace, posY, posX);
            }
            
            // If next position up is enemy, delete enemy, update enemy count.
            else if (GameSpace[posY][posX] == enemy) {
              GameSpace[posY][posX] = space;
              updateGameSpace(GameSpace, posY, posX);
              enemyCount--;

              // No more enemies, user wins.
              if (enemyCount == 0) {
                gameover = false;
                endgame = true;
              }
            }
          }

          // If missile reaches corner, eliminate the missile.
          else if (posY == 1){
            GameSpace[posY][posX] = space;
            updateGameSpace(GameSpace, posY, posX);
          }
        }
        
        // Current postion is where enemy missile is.
        else if (current == enemyMissile) {

          // User was hit. Reduce life points. Check if game over.
          if (GameSpace[posY + 1][posX] == spaceship) {
            
            lifepoints--;
            
            if (lifepoints == 0) {
              gameover = true;
              endgame = true;
            }
          }

          // If missile is in between, advance missile one more towards bottom corner.
          else if (posY < sizeY - 2 && GameSpace[posY + 1][posX] == space) {
            GameSpace[posY][posX] = space;
            updateGameSpace(GameSpace, posY, posX);
            
            posY++;
            
            GameSpace[posY][posX] = enemyMissile;
            updateGameSpace(GameSpace, posY, posX);
          }

          else if(GameSpace[posY + 1][posX] == border){
            GameSpace[posY][posX] = space;
            updateGameSpace(GameSpace, posY, posX);   
          }
          // Enemy missile reached bottom corner without hitting user. Erace enemy missile
          else if (posY == (sizeY - 2) && GameSpace[posY][posX] != spaceship)
            GameSpace[posY][posX] = space;
            updateGameSpace(GameSpace, posY, posX);
        }
      
      }
    }
  
    delay(gamespeed);
    // Update game space for each arrat element
    //updateGameSpace(GameSpace);
    
  }
}

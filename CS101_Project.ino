#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"

#define TFT_DC 9
#define TFT_CS 10

#define SNAKE_SIZE     12
#define SNAKE_POINT    -1
#define SNAKE_WALL     -2
#define SCREEN_SIZE_X 240
#define SCREEN_SIZE_Y 320

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

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

  /*
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
  */
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
      tft.print("Flappy Bird");
      tft.fillRect(25, 260, 190, 50, ILI9341_WHITE);
      tft.setCursor(40,280);
      tft.print("Space Invaders");
}

void TicTacToe() {
  int board[9] = { 0 };
  int i, position, turn, winner;
  boolean compPlayer = false;

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

      // Glitch where position 0 is triggered by touching the bottom of the screen
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

void Snake() {
  // create map and player
  char snake_map[SNAKE_SIZE][SNAKE_SIZE];
  Player p;

  // put walls around the border
  for(int i=0; i<SNAKE_SIZE; ++i) {
    snake_map[         0][         i] = SNAKE_WALL;
    snake_map[SNAKE_SIZE][         i] = SNAKE_WALL;
    snake_map[         i][SNAKE_SIZE] = SNAKE_WALL;
    snake_map[         i][         0] = SNAKE_WALL;
  }

  // put the player on the map
  p.x = p.y = SNAKE_SIZE/2;
  snake_map[p.y][p.x] = 1;

  // start loop
  snake_main_loop(snake_map, p);
}

void snake_main_loop(char **snake_map, Player& p) {
  do {
    snake_display(snake_map, p.score);
    snake_process_input(p);
    delay(50); // TODO - not sure about the arduino API
  } while(snake_move(snake_map, p));

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(30,150);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Game over!");
  delay(2000);
}

// displays the map
//   returns false on game over
void snake_display(char **snake_map, const int& score) {
  const int SCREEN_SIZE_X 240; // TODO this value may need testing
  const int SCREEN_SIZE_Y 320;
  int width    = (SCREEN_SIZE_X>SCREEN_SIZE_Y?SCREEN_SIZE_Y:SCREEN_SIZE_X)/SNAKE_SIZE;
  int offset_x = SCREEN_SIZE_X > SCREEN_SIZE_Y ? SCREEN_SIZE_X - SCREEN_SIZE_Y;
  int offset_y = SCREEN_SIZE_X < SCREEN_SIZE_Y ? SCREEN_SIZE_Y - SCREEN_SIZE_X;
  int colour   = ILI9341_GREEN;

  tft.fillScreen(ILI9341_BLACK);  // TODO - not sure about the arduino API
  tft.setCursor(30,150);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("Score: %4d", score);
  delay(2000);
  //tft.printNumI(score, 4, 4);   // TODO - not sure about the arduino API

  for(int i=0; i<SNAKE_SIZE; ++i) {
    for(int j=0; j<SNAKE_SIZE; ++j) {
      if(snake_map[j][i] == SNAKE_POINT)      // point
        colour = ILI9341_RED;    //   TODO - not sure about the arduino API
      else if(snake_map[j][i] == SNAKE_WALL) // wall
        colour = ILI9341_BLUE;   //   TODO - not sure about the arduino API
      else if(snake_map[j][i] > 0)   // player
        colour = ILI9341_GREEN;  //   TODO - not sure about the arduino API
      tft.fillRect(i*width+offset_x, j*width+offset_y, width, width, colour);
    }
  }
}

bool snake_move(char **snake_map, Player& p) {
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

  // update the map - "move the player"
  for(int i=0; i<SNAKE_SIZE; ++i) {
    for(int j=0; j<SNAKE_SIZE; ++j) {
      if(snake_map[j][i] > 0)
        ++snake_map[j][i];
      else if(snake_map[j][i] == SNAKE_POINT)
        ++nPoins;
      if(snake_map[j][i] > p.score+4)
        snake_map[j][i] = 0;
    }
  }

  // Put poins on the field if none are there
  while(!nPoints) {
    x = random() % (SNAKE_SIZE-2) + 1; // account for outer walls
    y = random() % (SNAKE_SIZE-2) + 1;
    if (snake_map[x][y] == 0) {
      snake_map[x][y] = SNAKE_POINT;
      ++nPoints;
    }
  }

  // set the head of the player to their current location
  snake_map[p.y][p.x] = 1;
  return true;
}

void snake_process_input(Player& player) {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    p.x = map(p.x, 0, 240, 500, 0);
    p.y = map(p.y, 0, 320, 500, 0);

    // split the screen up into an x and check for inputs on each size of it
    if(p.x > p.y) {   // top right
      if(500 - p.x > p.y) { // TOP
        player.dir = UP;
      }
      else {            // RIGHT
        player.dir = RIGHT;
      }
    }
    else { // bottom left
      if(500 - p.x > p.y) { // BOTTOM
        player.dir = DOWN;
      }
      else {            // LEFT
        player.dir = LEFT;
      }
    }
  }
}

void FlappyBird() {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(1);
      tft.setCursor(30,30);
      tft.print("Flappy Bird goes here");
      delay(2000);
}

void SpaceInvaders() {
      tft.fillScreen(ILI9341_BLACK);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(1);
      tft.setCursor(30,30);
      tft.print("Space Invaders goes here");
      delay(2000);
}

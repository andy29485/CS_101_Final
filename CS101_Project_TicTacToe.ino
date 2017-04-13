#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Adafruit_FT6206.h"

#define TFT_DC 9
#define TFT_CS 10

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206 ts = Adafruit_FT6206();

/*
 * Brendan's notes: 
 * 1 - You need all three of the Adafruit libraries included above to run this; all are available on the Adafruit site
 * 2 - I will have the splash screen and main menu finished within a couple of days
 * 3 - I am still making minor cosmetic adjustments and optimizations; the code here is simply a working demonstration
 */

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
}

void loop() {
    // TODO: Menu will go here eventually; for now Tic Tac Toe simply restarts after every game
    TicTacToe();
}

void TicTacToe() {
  int board[9] = { 0 }; // Board has postions from 0-8, with the top left corner at 0, top middle at 1, top right at 2, and so on
  int i, position, turn, winner; // i: counts number of turns, position: one of 9 possible positions on the board, turn: 1 for Player 1, 2 for Player 2, winner: 1 for Player 1, 2 for Player 2 
  
  drawBoard();
  turn = 1; // Player 1 goes first by default

  // Proceed through, at most, 9 turns
  for (i = 0; i < 9; i++) {
    // If it is Player 1's turn...
    if (turn == 1) {
        // Display that is is Player 1's turn (fillRect is used to cover over previous label after each turn so that the entire scene does not need to be redrawn)
        tft.fillRect(25, 270, 110, 20, ILI9341_BLACK);
        tft.setCursor(30,280);
        tft.setTextSize(1);
        tft.print("Player 1's turn");
        
        // Get the position on the board that the player selects; do not proceed until player selects an open position
        do {
        position = getInput();
        } while (board[position] != 0);
        // An 'O' is represented by the position on the 'board' being assigned 1
        board[position] = 1;
        // Draw the 'O' on the display
        drawPlayer1(position);
        // Pause necessary to prevent continous input on the touchscreen
        delay(500);
    }
    // Same as above, except for Player 2
    else if (turn == 2) {
        tft.fillRect(25, 270, 110, 15, ILI9341_BLACK);
        tft.setCursor(30,280);
        tft.setTextSize(1);
        tft.print("Player 2's turn");
        
        do {
        position = getInput();
        } while (board[position] != 0);
        // An 'X' is represented by the position on the 'board' being assigned 2
        board[position] = 2;
        drawPlayer2(position);
        delay(500);
    }

    // Check to see if either player has made a winning move
    winner = winConditions(board);

    // If Player 1 wins, display victory message and exit the for loop
    if (winner == 1) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("Player 1 Wins!");
        delay(1000);
        break;
    }
    // Same as above, for Player 2
    else if (winner == 2) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("Player 2 Wins!"); 
        delay(1000);
        break;            
    }

    // Alternate players at the end of each turn
    if (turn == 1)
      turn = 2;
    else
      turn = 1;
  }
    // If neither player has won after 9 turns, display tie message
    if (winner == 0) {
        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(30,150);
        tft.setTextColor(ILI9341_WHITE);
        tft.setTextSize(2);
        tft.print("It's a tie!"); 
        delay(1000);  
    }
}

int getInput() {
  int touch = 0, position = 0;

  // If the screen is touched within one of nine allowable areas, record position
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
        
      // Once the screen has been touched, exit loop
      touch = 1;
    }
  }
  return position;
}

void drawBoard() {
  // Displays the Tic Tac Toe board
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

// Display an 'O' at the appropriate postion
void drawPlayer1(int position) {
    switch(position) {
    case 0: tft.drawCircle(60,85,20,ILI9341_BLUE);  break;
    case 1: tft.drawCircle(120,85,20,ILI9341_BLUE); break;
    case 2: tft.drawCircle(180,85,20,ILI9341_BLUE); break;
    case 3: tft.drawCircle(60,150,20,ILI9341_BLUE);  break;
    case 4: tft.drawCircle(120,150,20,ILI9341_BLUE); break;
    case 5: tft.drawCircle(180,150,20,ILI9341_BLUE); break;
    case 6: tft.drawCircle(60,210,20,ILI9341_BLUE); break;
    case 7: tft.drawCircle(120,210,20,ILI9341_BLUE);break;
    case 8: tft.drawCircle(180,210,20,ILI9341_BLUE);break;
  }
}

// Display an 'X' at the appropriate postion
void drawPlayer2(int position) {
    switch(position) {
    case 0: drawX(60,85);  break;
    case 1: drawX(120,85); break;
    case 2: drawX(180,85); break;
    case 3: drawX(60,150);  break;
    case 4: drawX(120,150); break;
    case 5: drawX(180,150); break;
    case 6: drawX(60,210); break;
    case 7: drawX(120,210);break;
    case 8: drawX(180,210);break;
  }
}

int winConditions(int board[]) {
  int winner = 0;

  // Win conditions for Player 1
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

  // Win conditions for Player 2
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

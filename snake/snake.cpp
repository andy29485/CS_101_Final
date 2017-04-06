//CS101 Final Project
//Group:   12
//file:    snake.cpp
//date:    2017-03-25
//authors: Andriy Zasypkim

#import "snake.hpp"
#define SIZE           12;
#define POINT          -1;
#define WALL           -2;
#define DELAY_TIME    600; // TODO this value may need testing
#define SCREEN_SIZE_X 300; // TODO this value may need testing
#define SCREEN_SIZE_Y 300; // TODO this value may need testing

void start_snake() {
  // create map and player
  char map[SIZE][SIZE]; //only need char b/c value range will be -2 - +100
  Player p;

  // put walls around the border
  for(int i=0; i<SIZE; ++i) {
    map[   0][   i] = WALL;
    map[SIZE][   i] = WALL;
    map[   i][SIZE] = WALL;
    map[   i][   0] = WALL;
  }

  // put the player on the map
  p.x = p.y = SIZE/2;
  map[p.y][p.x] = 1;

  // start loop
  main_loop(map, p);
}

// displays the map
//   returns false on game over
void display(char **map, const int& score) {
  int width    = (SCREEN_SIZE_X>SCREEN_SIZE_Y?SCREEN_SIZE_Y:SCREEN_SIZE_X)/SIZE;
  int offset_x = SCREEN_SIZE_X > SCREEN_SIZE_Y ? SCREEN_SIZE_X - SCREEN_SIZE_Y;
  int offset_y = SCREEN_SIZE_X < SCREEN_SIZE_Y ? SCREEN_SIZE_Y - SCREEN_SIZE_X;

  clrScr();               // TODO - not sure about the arduino API
  printNumI(score, 4, 4); // TODO - not sure about the arduino API


  for(int i=0; i<SIZE; ++i) {
    for(int j=0; j<SIZE; ++j) {
      if(map[j][i] == POINT)     // point
        setColor(63, 180, 200);  //   TODO - not sure about the arduino API
      else if(map[j][i] == WALL) // wall
        setColor(10, 20, 255);   //   TODO - not sure about the arduino API
      else if(map[j][i] > 0)     // player
        setColor(255, 255, 255); //   TODO - not sure about the arduino API
      fillRect(i*windth + offset_x, j*width+offset_y, width, width);
    }
  }
}

void main_loop(char **map, Player& p) {
  do {
    display(map, p.score);
    process_input(p);
    delay(DELAY_TIME); // TODO - not sure about the arduino API
  } while(move(map, p));
}

bool move(char **map, Player& p) {
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
  if(map[p.y][p.x] == POINT) {   // point get
    ++p.score;
  }
  else if(map[p.y][p.x] != 0) {  // crash
    return false
  }

  // update the map - "move the player"
  for(int i=0; i<SIZE; ++i) {
    for(int j=0; j<SIZE; ++j) {
      if(map[j][i] > 0)
        ++map[j][i];
      else if(map[j][i] == POINT)
        ++nPoins;
      if(map[j][i] > p.score+4)
        map[j][i] = 0;
    }
  }

  // Put poins on the field if none are there
  while(!nPoints) {
    x = random() % (SIZE-2) + 1; // account for outer walls
    y = random() % (SIZE-2) + 1;
    if (map[x][y] == 0) {
      map[x][y] = POINT;
      ++nPoints;
    }
  }

  // set the head of the player to their current location
  map[p.y][p.x] = 1;
  return true;
}

void process_input(Player& p) {
  // retrive coordinates
  int x = getX() * 500 / SCREEN_SIZE_X; // TODO - not sure about the arduino API
  int y = getY() * 500 / SCREEN_SIZE_Y; // TODO - not sure about the arduino API

  // split the screen up into an x and check for inputs on each size of it

  if(x > y) {   // top right
    if(500 - x > y) { // TOP
      p.dir = UP;
    }
    else {            // LEFT
      p.dir = LEFT;
    }
  }
  else { // bottom left
    if(500 - x > y) { // BOTTOM
      p.dir = DOWN;
    }
    else {            // RIGHT
      p.dir = RIGHT;
    }
  }
}

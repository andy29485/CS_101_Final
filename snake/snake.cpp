//CS101 Final Project
//Group:   12
//file:    snake.cpp
//date:    2017-03-25
//authors: Andriy Zasypkim

#import "snake.hpp"
#define SIZE  12;
#define POINT -1;
#define WALL  -2;

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
}

void display(char **map) {
  // TODO display map
}

void move(char **map, Player p) {
  //TODO move
  //TODO check if game is over
  //TODO add to score
  //TODO update map
}

void process_input(Player p) {
  // TODO change dir based on input
}

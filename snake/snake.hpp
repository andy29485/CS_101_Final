//CS101 Final Project
//Group:   12
//file:    snake.hpp
//date:    2017-03-25
//authors: Andriy Zasypkim

#ifndef SNAKE_HPP
#define SNAKE_HPP

enum Direction {
  LEFT,
  UP,
  RIGHT,
  DOWN
};

typedef struct {
  int x;
  int y;
  int score;
  Direction dir;
} Player;

void start_snake();


#endif

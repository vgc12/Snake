#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <termios.h>
#include <limits>
#include <thread>
#include <chrono>

// Game dimensions
const int width = 20;
const int height = 20;

// Direction enum
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

// Node structure for linked list
struct Node {
    int x, y;
    Node* next;
};

// Snake linked list
class Snake {
  public:
    Node* head;  // The head of the snake
    Node* tail;  // The tail of the snake
  
    void newHead(int newX, int newY);
    void freeSnake(); 
};

// Game logic is handled here
class GameLogic{
  public:
    bool gameOver;
    int fruitX, fruitY, score;
    Snake snake;
    Direction dir;
  
    void setup();
    void draw();
    void input();
    void moveSnake();
    void clearScreen();
    char getch();
};


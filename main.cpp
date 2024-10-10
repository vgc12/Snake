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

// Clear the screen (using ANSI escape sequences)
void clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}

// Game dimensions
const int width = 20;
const int height = 20;
char keyInput;
bool gameOver;
int x, y, fruitX, fruitY, score;
std::vector<std::pair<int, int>> snake;
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void setup() {
    gameOver = false;
    dir = RIGHT;  // Default movement direction
    x = width / 2;
    y = height / 2;
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
    snake.clear();
    snake.push_back({x, y});  // Start with the snake having 1 segment at the center
}

void draw() {
    clearScreen();
    for (int i = 0; i < width + 2; i++)
        std::cout << "#";  // Top border
    std::cout << std::endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0)
                std::cout << "#";  // Left border

            if (i == y && j == x)
                std::cout << "O";  // Snake head
            else if (i == fruitY && j == fruitX)
                std::cout << "F";  // Fruit
            else {
                bool isBodyPart = false;
                // Check if any part of the snake is here
                for (int k = 0; k < snake.size(); k++) {
                    if (snake[k].first == j && snake[k].second == i) {
                        std::cout << "o";  // Snake body
                        isBodyPart = true;
                    }
                }
                if (!isBodyPart)
                    std::cout << " ";  // Empty space
            }

            if (j == width - 1)
                std::cout << "#";  // Right border
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; i++)
        std::cout << "#";  // Bottom border
    std::cout << std::endl;

    std::cout << "Score: " << score << std::endl;
}


char getch() {


    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 0;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror ("read()");


    if (read(0, &buf, 1) < 0) {
        perror("read()");

    }

    std::cout<<buf<<std::endl;
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror ("tcsetattr ~ICANON");
    return (buf);
}



void input() {

    char keyInput = getch();

    switch (keyInput) {
        case 'a':
            if (dir != RIGHT) dir = LEFT;
            break;
        case 'd':
            if (dir != LEFT) dir = RIGHT;
            break;
        case 'w':
            if (dir != DOWN) dir = UP;
            break;
        case 's':
            if (dir != UP) dir = DOWN;
            break;
        case 'q':
            gameOver = true;
            break;
    }

}

void logic() {
    // Update the snake's body position
    int prevX = snake[0].first;
    int prevY = snake[0].second;
    int prev2X, prev2Y;

    snake[0].first = x;
    snake[0].second = y;

    // Move the body following the head
    for (int i = 1; i < snake.size(); i++) {
        prev2X = snake[i].first;
        prev2Y = snake[i].second;
        snake[i].first = prevX;
        snake[i].second = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    // Move the head of the snake
    switch (dir) {
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

    // Check if snake hit the wall
    if (x >= width) x = 0; else if (x < 0) x = width - 1;
    if (y >= height) y = 0; else if (y < 0) y = height - 1;

    // Check if snake collided with itself
    for (int i = 1; i < snake.size(); i++) {
        if (snake[i].first == x && snake[i].second == y) {
            gameOver = true;
        }
    }

    // Check if snake ate the fruit
    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        snake.push_back({prevX, prevY});  // Grow the snake at the tail
    }
}

float deltaTime = 0.0f;
std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

int main() {
    srand(time(0));
    setup();
    int frame = 0; // total frames since game start

    while (!gameOver) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        draw();
        input();
        logic();
        std::cout<<frame<<std::endl;

        frame++;
        if (deltaTime < (1.0f / 144.0f)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));

        }

    }

    std::cout << "Game Over!" << std::endl;
    return 0;
}


#include "header.h"  


////////////////////////////////////////////////////////////////////////////// GAME FUNCTIONS
////////////////////////////////////////////////////////////////////////////// GAME FUNCTIONS
////////////////////////////////////////////////////////////////////////////// GAME FUNCTIONS



void GameLogic::setup() {
    gameOver = false;
    dir = RIGHT;  // Default movement direction
    score = 0;

    // Initialize the snake with one segment in the middle of the game area
    snake.head = new Node;
    snake.head->x = width / 2;
    snake.head->y = height / 2;
    snake.head->next = nullptr;
    snake.tail = snake.head;

    // Initialize the fruit in a random position
    fruitX = rand() % width;
    fruitY = rand() % height;
}

// Draw the game state
void GameLogic::draw() {
    clearScreen();
    for (int i = 0; i < width + 2; i++) {
        std::cout << "#";  // Top border
    }
    std::cout << std::endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) {
                std::cout << "#";  // Left border
            }

            bool printed = false;

            // Check if we should print the snake
            Node* current = snake.head;
            while (current != nullptr) {
                if (current->x == j && current->y == i) {
                    std::cout << (current == snake.head ? "O" : "o");  // Head is 'O', body is 'o'
                    printed = true;
                    break;
                }
                current = current->next;
            }

            if (!printed) {
                // Print fruit
                if (i == fruitY && j == fruitX) {
                    std::cout << "F";
                } else {
                    std::cout << " ";  // Empty space
                }
            }

            if (j == width - 1) {
                std::cout << "#";  // Right border
            }
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < width + 2; i++) {
        std::cout << "#";  // Bottom border
    }
    std::cout << std::endl;

    std::cout << "Score: " << score << std::endl;
}


char GameLogic::getch() {
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


// Handle input from the player
void GameLogic::input() {

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


// Move the snake in the current direction
void GameLogic::moveSnake() {
    // Calculate the new position of the head
    int newX = snake.head->x;
    int newY = snake.head->y;

    switch (dir) {
        case LEFT:
            newX--;
            break;
        case RIGHT:
            newX++;
            break;
        case UP:
            newY--;
            break;
        case DOWN:
            newY++;
            break;
        default:
            break;
    }

    // Check for wall collisions (snake wraps around)
    if (newX >= width) newX = 0; else if (newX < 0) newX = width - 1;
    if (newY >= height) newY = 0; else if (newY < 0) newY = height - 1;

    // Check if the snake hits itself
    Node* current = snake.head;
    while (current != nullptr) {
        if (current->x == newX && current->y == newY) {
            gameOver = true;
            return;
        }
        current = current->next;
    }

    snake.newHead(newX,newY);

    // Check if the snake eats the fruit
    if (newX == fruitX && newY == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
    } else {
        // If not eating, remove the tail
        Node* temp = snake.head;
        while (temp->next != snake.tail) {
            temp = temp->next;
        }
        delete snake.tail;
        snake.tail = temp;
        snake.tail->next = nullptr;
    }
}

// Clear the screen (using ANSI escape sequences)
void GameLogic::clearScreen() {
    std::cout << "\033[2J\033[1;1H";
}


////////////////////////////////////////////////////////////////////////////// SNAKE FUNCTIONS
////////////////////////////////////////////////////////////////////////////// SNAKE FUNCTIONS
////////////////////////////////////////////////////////////////////////////// SNAKE FUNCTIONS

// Free the snake memory
void Snake::freeSnake() {
    Node* current = head;
    while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
    }
}

void Snake::newHead(int newX, int newY){
  // Add a new head at the new position
    Node* newHead = new Node;
    newHead->x = newX;
    newHead->y = newY;
    newHead->next = head;
    head = newHead;
}
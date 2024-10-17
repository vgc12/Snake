#include "library.cpp"  

float deltaTime = 0.0f;
std::chrono::time_point<std::chrono::high_resolution_clock> lastTime = std::chrono::high_resolution_clock::now();

int main() {
  
    GameLogic game;
  
    srand(time(0));
    game.setup();
    int frame = 0; // total frames since game start

    while (!game.gameOver) {

        auto currentTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
        lastTime = currentTime;

        game.draw();
        game.input();
        game.moveSnake();
        std::cout<<frame<<std::endl;
 
        frame++;
        if (deltaTime < (1.0f / 1000.0f)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

        }
  
    }

    std::cout << "Game Over!" << std::endl;
    return 0; 
}

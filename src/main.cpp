#include "raylib.h"
#include <array>
#include <iostream>

const int windowWidth = 800;
const int windowHeight = 600;
const int blockSize = 20;
const int tickRate = 10; // Game speed (lower is faster)
const int rows = windowHeight / blockSize;
const int cols = windowWidth / blockSize;

enum GameState {
    MENU,
    PLAYING,
    GAMEOVER
};

enum Direction {
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

struct IntVector2 {
    int x;
    int y;
};

class Food {
    public:
        IntVector2 position;
        Color color = RED;

        void respawn() {
            position.x = GetRandomValue(0, cols - 1);
            position.y = GetRandomValue(0, rows - 1);
        }

        void draw() {
            DrawRectangle(position.x * blockSize, position.y * blockSize, blockSize, blockSize, color);
        }
};

class Snake {
    public:
        int length = 3;
        int speed = 1;
        Color color = GREEN;
        Direction direction = Direction::NONE;
        std::array<IntVector2, rows * cols> snakePositions;

        void spawn() {
            length = 3;
            direction = Direction::NONE;
            for (int i = 0; i < length; i++) {
                snakePositions[i] = {cols / 2 - i, rows / 2};
            }
        }

        void addSegment() {
            length++;
            snakePositions[length - 1] = snakePositions[length - 2];
        }

        void move() {
            for (int i = length - 1; i > 0; i--) {
                snakePositions[i] = snakePositions[i-1];
            }
            
            if (direction == Direction::UP) {
                snakePositions[0].y -= speed;
            } else if (direction == Direction::DOWN) {
                snakePositions[0].y += speed;
            } else if (direction == Direction::LEFT) {
                snakePositions[0].x -= speed;
            } else if (direction == Direction::RIGHT) {
                snakePositions[0].x += speed;
            }
        }

        void update(Food* food) {
            // Food Collision
            if (snakePositions[0].x == food->position.x && snakePositions[0].y == food->position.y) {
                addSegment();
                food->respawn();
            }

            // Wall Collision
            if (snakePositions[0].x < 0 || snakePositions[0].x >= cols || snakePositions[0].y < 0 || snakePositions[0].y >= rows) {
                spawn();
            }

            // Self Collision
            for (int i = 1; i < length; i++) {
                if (snakePositions[0].x == snakePositions[i].x && snakePositions[0].y == snakePositions[i].y) {
                    spawn();
                    break;
                }
            }
        }

        void draw() {
            for (int i = 0; i < length; i++) {
                if (i == 0) {
                    color = DARKGREEN;
                } else {
                    color = GREEN;
                }
                DrawRectangle(snakePositions[i].x * blockSize, snakePositions[i].y * blockSize, blockSize, blockSize, color);
            }
        }
};

Snake snake;
Food food;

int tickCount = 0;

GameState gameState = GameState::MENU;

int main() {
    InitWindow(800, 600, "Snake");
    SetTargetFPS(60);

    snake.spawn();
    food.respawn();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        // Game logic
        
        if (IsKeyPressed(KEY_UP) && snake.direction != Direction::DOWN) {
            snake.direction = Direction::UP;
        } else if (IsKeyPressed(KEY_DOWN) && snake.direction != Direction::UP) {
            snake.direction = Direction::DOWN;
        } else if (IsKeyPressed(KEY_LEFT) && snake.direction != Direction::RIGHT && snake.direction != Direction::NONE) {
            snake.direction = Direction::LEFT;
        } else if (IsKeyPressed(KEY_RIGHT) && snake.direction != Direction::LEFT) {
            snake.direction = Direction::RIGHT;
        }

        tickCount++;
        if ((tickCount % tickRate) == 0) {
            snake.move();
            snake.update(&food);
        }
        
        // Game rendering
        snake.draw();
        food.draw();

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
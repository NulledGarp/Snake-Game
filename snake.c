#include <windows.h>
#include <stdio.h>
#include <conio.h>

#define WIDTH  40
#define HEIGHT 20
#define MAXLEN 100

typedef struct { int x, y; } Point;

Point snake[MAXLEN];
int length = 5;
int dx = 1, dy = 0;
Point food;

HANDLE hConsole;

// Move console cursor
void gotoXY(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(hConsole, c);
}

// Draw game border
void drawBorder() {
    for (int x = 0; x <= WIDTH; x++) {
        gotoXY(x, 0);     putchar('#');
        gotoXY(x, HEIGHT);putchar('#');
    }
    for (int y = 0; y <= HEIGHT; y++) {
        gotoXY(0, y);     putchar('#');
        gotoXY(WIDTH, y); putchar('#');
    }
}

// Simple pseudo-random generator
unsigned int seed = 1;
int simpleRand() {
    seed = (214013 * seed + 2531011);
    return (seed >> 16) & 0x7FFF;
}

// Spawn food in empty space
void spawnFood() {
    food.x = (simpleRand() % (WIDTH - 2)) + 1;
    food.y = (simpleRand() % (HEIGHT - 2)) + 1;
}

// Draw snake on screen
void drawSnake() {
    for (int i = 0; i < length; i++) {
        gotoXY(snake[i].x, snake[i].y);
        putchar(i == 0 ? 'O' : 'o');
    }
}

// Clear snake's tail
void eraseTail() {
    gotoXY(snake[length - 1].x, snake[length - 1].y);
    putchar(' ');
}

// Move snake forward
void moveSnake() {
    for (int i = length - 1; i > 0; i--)
        snake[i] = snake[i - 1];
    snake[0].x += dx;
    snake[0].y += dy;
}

// Check for collisions with wall or self
int checkCollision() {
    if (snake[0].x <= 0 || snake[0].x >= WIDTH || snake[0].y <= 0 || snake[0].y >= HEIGHT)
        return 1;
    for (int i = 1; i < length; i++)
        if (snake[i].x == snake[0].x && snake[i].y == snake[0].y)
            return 1;
    return 0;
}

// Read arrow key input
void handleInput() {
    if (GetAsyncKeyState(VK_UP)    & 0x8000 && dy != 1)  { dx = 0; dy = -1; }
    if (GetAsyncKeyState(VK_DOWN)  & 0x8000 && dy != -1) { dx = 0; dy = 1; }
    if (GetAsyncKeyState(VK_LEFT)  & 0x8000 && dx != 1)  { dx = -1; dy = 0; }
    if (GetAsyncKeyState(VK_RIGHT) & 0x8000 && dx != -1) { dx = 1; dy = 0; }
}

// game over message
void gameOverScreen() {
    gotoXY(WIDTH / 2 - 5, HEIGHT / 2);
    printf("GAME OVER");
}

// itle screen
void showTitle() {
    system("cls");
    gotoXY(WIDTH / 2 - 6, HEIGHT / 2 - 2);
    printf("=== BROKE MAN SNAKE ===");
    gotoXY(WIDTH / 2 - 11, HEIGHT / 2);
    printf("Press any key to begin...");
    while (!_kbhit()); // Wait for key press
    system("cls");
}

int main() {
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    showTitle();

    // Initialize snake in center
    for (int i = 0; i < length; i++) {
        snake[i].x = WIDTH / 2 - i;
        snake[i].y = HEIGHT / 2;
    }

    drawBorder();
    spawnFood();

    while (1) {
        handleInput();
        eraseTail();
        moveSnake();

        if (checkCollision()) {
            gameOverScreen();
            break;
        }

        // Food eaten var
        if (snake[0].x == food.x && snake[0].y == food.y) {
            if (length < MAXLEN) length++;
            spawnFood();
        }

        gotoXY(food.x, food.y); putchar('@');
        drawSnake();
        Sleep(100);
    }

    gotoXY(0, HEIGHT + 1);
    return 0;
}

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "./libraries/raylib/raylib.h"




#define WINSIZE 1000 

#define WIDTH 50
#define HEIGHT 50

#define SCALE (WINSIZE / min(WIDTH, HEIGHT))

int min(int a, int b) {
    if(a < b) {
        return a;
    } else {
        return b;
    }
}

void drawRect(int x, int y) {
    DrawRectangle(x * SCALE, y * SCALE, SCALE, SCALE, WHITE);
}

typedef bool Cells[WIDTH][HEIGHT];

Cells current;
Cells next;

//rules for if the current is dead
bool dead_neighbors[9] = {
    false,
    false,
    false,
    true,
};
// rules for if the current is alive
bool alive_neighbors[9] = {
    false,
    false,
    true,
    true,
};

int wrap(int a, int b) {
    return (a%b + b)%b;
}

int count_neighbors(int x, int y) {
    int n = 0;
    for(int xx = -1; xx <= 1; xx++) {
        for(int yy = -1; yy <= 1; yy++) {
            if(xx == 0 && yy == 0) {
                continue;
            }
            int sx = wrap(xx + x, WIDTH);
            int sy = wrap(yy + y, HEIGHT);
            if(current[sy][sx]) {
                n++;
            }
        }
    }
    return n;
}

void apply(bool alive, int neighbors, bool* next) {
    if(alive) {
        *next = alive_neighbors[neighbors];
    } else {
        *next = dead_neighbors[neighbors];
    }
}

void cpy(Cells a, Cells b) {
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            a[y][x] = b[y][x];
        }
    }
}

void step() {
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            apply(current[y][x], count_neighbors(x, y), &next[y][x]); 
        }
    }
    cpy(current, next);
}

void view() {
    for(int x = 0; x < WIDTH; x++) {
        for(int y = 0; y < HEIGHT; y++) {
            if(current[y][x]) {
                drawRect(x, y);      
            }
        }
    }
}

void setup() {
    InitWindow(WIDTH * SCALE, HEIGHT * SCALE, "Conway's Game Of Life");
}

Vector2 getPos() {
    Vector2 pos = GetMousePosition();
    pos.x = floorf(pos.x / SCALE);
    pos.y = floorf(pos.y / SCALE);
    return pos;
}

bool key(int key) {
    return IsKeyPressed(key) || IsKeyPressedRepeat(key);
}

void draw() {
    ClearBackground(GetColor(0x000000ff));
    view();
    if(key(KEY_ENTER) || key(KEY_SPACE)) {
        step();
    }
    Vector2 pos = getPos();
    DrawRectangle(pos.x * SCALE, pos.y * SCALE, SCALE, SCALE, GetColor(0x47474777));
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        current[(int)pos.y][(int)pos.x] = !current[(int)pos.y][(int)pos.x];
    }
}

int main() {
    setup();
    while(!WindowShouldClose()) {
        BeginDrawing(); 
        draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

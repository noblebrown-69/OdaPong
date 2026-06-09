#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

// Constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PADDLE_WIDTH = 10;
const int PADDLE_HEIGHT = 100;
const int BALL_SIZE = 10;
const int PADDLE_SPEED = 5;
const int BALL_SPEED_X_INIT = 4;
const int BALL_SPEED_Y_INIT = 4;

struct Entity {
    float x, y;
    float w, h;
    float dx, dy;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Entity paddle1 = {20, (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f), (float)PADDLE_WIDTH, (float)PADDLE_HEIGHT, 0, 0};
    Entity paddle2 = {SCREEN_WIDTH - 20 - PADDLE_WIDTH, (SCREEN_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f), (float)PADDLE_WIDTH, (float)PADDLE_HEIGHT, 0, 0};
    Entity ball = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT / 2, (float)BALL_SIZE, (float)BALL_SIZE, (float)BALL_SPEED_X_INIT, (float)BALL_SPEED_Y_INIT};

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Input handling
        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
        if (currentKeyStates[SDL_SCANCODE_W] && paddle1.y > 0) paddle1.y -= PADDLE_SPEED;
        if (currentKeyStates[SDL_SCANCODE_S] && paddle1.y < SCREEN_HEIGHT - paddle1.h) paddle1.y += PADDLE_SPEED;
        if (currentKeyStates[SDL_SCANCODE_UP] && paddle2.y > 0) paddle2.y -= PADDLE_SPEED;
        if (currentKeyStates[SDL_SCANCODE_DOWN] && paddle2.y < SCREEN_HEIGHT - paddle2.h) paddle2.y += PADDLE_SPEED;

        // Ball movement
        ball.x += ball.dx;
        ball.y += ball.dy;

        // Ball collision with top and bottom
        if (ball.y <= 0 || ball.y + ball.h >= SCREEN_HEIGHT) {
            ball.dy = -ball.dy;
        }

        // Ball collision with paddles
        SDL_Rect ballRect = {(int)ball.x, (int)ball.y, (int)ball.w, (int)ball.h};
        SDL_Rect p1Rect = {(int)paddle1.x, (int)paddle1.y, (int)paddle1.w, (int)paddle1.h};
        SDL_Rect p2Rect = {(int)paddle2.x, (int)paddle2.y, (int)paddle2.w, (int)paddle2.h};

        if (SDL_HasIntersection(&ballRect, &p1Rect) || SDL_HasIntersection(&ballRect, &p2Rect)) {
            ball.dx = -ball.dx;
        }

        // Ball out of bounds (reset)
        if (ball.x <= 0 || ball.x >= SCREEN_WIDTH) {
            ball.x = SCREEN_WIDTH / 2.0f;
            ball.y = SCREEN_HEIGHT / 2.0f;
            ball.dx = -ball.dx; // Reverse direction on reset
        }

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &p1Rect);
        SDL_RenderFillRect(renderer, &p2Rect);
        SDL_RenderFillRect(renderer, &ballRect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // Approx 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
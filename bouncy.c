#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <math.h>

#define WIDTH 1200
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff
#define COLOR_BLACK 0x00000000
#define X_VELOCITY 2
#define Y_VELOCITY 1

struct Circle {
    double x;
    double y;
    double radius;
};

void drawCircle(SDL_Surface* surface, struct Circle circle, Uint32 color) {
    double radius_squared = pow(circle.radius, 2);
    for (double x = circle.x - circle.radius; x<=circle.x + circle.radius; x++) {
        for (double y = circle.y - circle.radius; y<=circle.y + circle.radius; y++) {
            double distance_squared = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if ( distance_squared < radius_squared ) {
                SDL_Rect pixel = (SDL_Rect) {x,y,1,1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int detectCollision(SDL_Surface* surface, struct Circle circle) {
    // top collision
    if (circle.y - circle.radius <= 0) return 0;
    // right collision
    if (circle.x + circle.radius >= surface->w) return 1;
    // bottom collision
    if (circle.y + circle.radius >= surface->h) return 2;
    // left collision
    if (circle.x - circle.radius <= 0) return 3;
    
    return -1;
}

int main (int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Bouncy Ball", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);

    SDL_Surface* surface = SDL_GetWindowSurface(window);

    struct Circle circle = {300,300,80};
    double x_speed = X_VELOCITY;
    double y_speed = Y_VELOCITY;

    SDL_Rect erase_rect = {0,0,WIDTH,HEIGHT};

    int sim_running = 1;
    SDL_Event event;
    while (sim_running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                sim_running = 0;
            } 
        }
        
        circle.x += x_speed;
        circle.y -= y_speed;

        if (detectCollision(surface, circle) == 0 || detectCollision(surface, circle) == 2) y_speed *= -1;
        if (detectCollision(surface, circle) == 1 || detectCollision(surface, circle) == 3) x_speed *= -1;

        SDL_FillRect(surface, &erase_rect, COLOR_BLACK);
        drawCircle(surface, circle, COLOR_WHITE);
        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }


    return 0;
}

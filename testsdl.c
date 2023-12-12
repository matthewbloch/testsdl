#include <stdio.h>
#include <math.h>
#include <SDL.h>
#ifdef __EMSCRIPTEN__
#  include "emscripten.h"
#endif

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Point trail[300];
int next_point = 0;

void main_loop() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_WINDOWEVENT:
            switch (event.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                printf("Window resized: %d, %d\n", event.window.data1, event.window.data2);
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                printf("Window focus lost\n");
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                printf("Window size changed: %d, %d\n", event.window.data1, event.window.data2);
                break;
            case SDL_WINDOWEVENT_ENTER:
                printf("Window enter\n");
                break;
            case SDL_WINDOWEVENT_LEAVE:
                printf("Window leave\n");
                break;
            default:
                printf("Window event: %d\n", event.window.event);
                break;
            }
            break;
        case SDL_QUIT:
            SDL_Quit();
            exit(0);
            break;
        case SDL_MOUSEBUTTONDOWN:
            printf("Mouse down: %d\n", event.button.button);
            switch (event.button.button)
            {
            case SDL_BUTTON_LEFT:
                do
                {
                    int mode0 = SDL_GetRelativeMouseMode();
                    int result = SDL_SetRelativeMouseMode(!mode0);
                    // This returns 0 even when it succeeds
                    printf("Set relative mouse mode: %d→%d %s\n", mode0, !mode0, result == SDL_TRUE ? "worked" : "didn't work");
                } while (0);
                break;
            case SDL_BUTTON_MIDDLE:
                SDL_SetWindowGrab(window, !SDL_GetWindowGrab(window));
                printf("Grab is now %s (assuming it worked)\n", SDL_GetWindowGrab(window) ? "on" : "off");
                break;
            case SDL_BUTTON_RIGHT:
                do
                {
                    int flags0 = SDL_GetWindowFlags(window) & (SDL_WINDOW_FULLSCREEN | SDL_WINDOW_FULLSCREEN_DESKTOP);
                    int flags1;
                    switch (flags0)
                    {
                    case 0:
                        flags1 = SDL_WINDOW_FULLSCREEN_DESKTOP;
                        break;
                    case SDL_WINDOW_FULLSCREEN_DESKTOP:
                        flags1 = SDL_WINDOW_FULLSCREEN;
                        break;
                    case SDL_WINDOW_FULLSCREEN:
                        flags1 = 0;
                        break;
                    }
                    int result = SDL_SetWindowFullscreen(window, flags1);
                    printf("Full screen setting %d→%d (result %d)\n", flags0, flags1, result);
                } while (0);
                break;
            case SDL_BUTTON_X2:
                printf("Warping mouse in window to 100,100\n");
                SDL_WarpMouseInWindow(window, 100, 100);
                break;
            }

            break;
        case SDL_MOUSEMOTION:
            //printf("Mouse motion: %d, %d (Δ %d,%d)\n", event.motion.x, event.motion.y, event.motion.xrel, event.motion.yrel);
            trail[next_point % 300].x = event.motion.x;
            trail[next_point % 300].y = event.motion.y;
            next_point++;
            break;
        default:
            printf("Event: %d\n", event.type);
            break;
        }
    }
    SDL_SetRenderDrawColor(renderer, 32, 32, 32, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    int wx,wy;
    SDL_GetWindowSize(window, &wx, &wy);
    SDL_RenderDrawLine(renderer, 0, 0, wx, wy);

    for (int i = 0; i < next_point; i++)
    {
        for (int j = 0; j < 10; j++) {
            SDL_SetRenderDrawColor(renderer, rand(), rand(), rand(), 255);
            float x = trail[i % 300].x;
            float y = trail[i % 300].y;
            float angle = (rand() / (float) RAND_MAX) * 2 * M_PI;
            float distance = (rand() / (float) RAND_MAX) * 2 * M_PI;
            x += cos(angle) * distance * j/2+1;
            y += sin(angle) * distance * j/2+1;
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    // hopefully this will wait for vsync and limit the CPU burn of this loop
    SDL_RenderPresent(renderer);
}


int main(int argc, char *argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Mouse capture test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    while(1) main_loop();
#endif
    return 0;
}

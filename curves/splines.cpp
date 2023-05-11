#include "curves.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

using namespace curves;

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("B-spline / Bezier (space to switch)",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event evt;

    std::vector<glm::vec2> control = {
        { 100.f, 300.f },
        { 100.f, 300.f },
        { 200.f, 350.f },
        { 250.f, 250.f },
        { 300.f, 280.f },
        { 400.f, 400.f },
        { 450.f, 420.f },
        { 500.f, 440.f },
        { 530.f, 380.f },
        { 490.f, 300.f },
        { 490.f, 300.f }
    };

    SplineType sptype = SplineType::BSPLINE;

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                running = false;
            if (evt.type == SDL_KEYDOWN)
                if (evt.key.keysym.sym == SDLK_SPACE)
                    sptype = sptype == SplineType::BSPLINE ?
                        SplineType::BEZIER : SplineType::BSPLINE;
        }

        SDL_RenderClear(rend);

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        for (size_t i = 0; i < control.size() - 1; ++i)
        {
            SDL_RenderDrawLine(rend, control[i].x, control[i].y,
                control[i + 1].x, control[i + 1].y);
        }

        SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
        draw_bspline(rend, control, sptype);

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

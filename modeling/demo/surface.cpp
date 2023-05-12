#include "curves.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

int main(int argc, char **argv)
{
    std::array<float, 16> my = {
        0.f, 0.f, 2.f, 0.f,
        2.f, 0.f, 2.f, 0.f,
        2.f, 0.f, 0.f, 2.f,
        0.f, 0.f, 0.f, 0.f
    };
    float inc = .005f;

    std::array<std::array<glm::vec3, 4>, 4> mp = {
        std::array<glm::vec3, 4>{ glm::vec3{ 0.f, my[0], 0.f }, { .33f, my[1], 0.f }, { .66f, my[2], 0.f }, { 1.f, my[3], 0.f } },
        std::array<glm::vec3, 4>{ glm::vec3{ 0.f, my[4], .33f }, { .33f, my[5], 0.33f }, { .66f, my[6], 0.33f }, { 1.f, my[7], 0.33f } },
        std::array<glm::vec3, 4>{ glm::vec3{ 0.f, my[8], .66f }, { .33f, my[9], 0.66f }, { .66f, my[10], 0.66f }, { 1.f, my[11], 0.66f } },
        std::array<glm::vec3, 4>{ glm::vec3{ 0.f, my[12], 1.f }, { .33f, my[13], 1.f }, { .66f, my[14], 1.f }, { 1.f, my[15], 1.f } }
    };

    // printf("%f\n", curves::bicubic_bezier_p_t(0.f, 0.f, mp));
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Bezier surface",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event evt;

    float pmax = 0.f;
    for (float u = 0.f; u <= 1.f; u += inc)
    {
        for (float v = 0.f; v <= 1.f; v += inc)
        {
            float y = curves::bicubic_bezier_p_t(u, v, mp);
            if (y > pmax)
                pmax = y;
        }
    }

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(rend);

        for (float u = 0.f; u <= 1.f; u += inc)
        {
            for (float v = 0.f; v <= 1.f; v += inc)
            {
                float y = curves::bicubic_bezier_p_t(u, v, mp);
                int intensity = (int)(y / pmax * 255.f);
                SDL_SetRenderDrawColor(rend, intensity, 0, 0, 255);
                SDL_RenderDrawPoint(rend, u * 600, v * 600);
            }
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

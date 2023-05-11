#include "animation.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

void render(SDL_Renderer *rend, animation::Joint *shoulder)
{
    glm::vec2 arm_pos = { 100.f, 100.f };
    glm::vec2 prev_pos = arm_pos;

    animation::Joint *j = shoulder;

    int color = 255;
    while (j)
    {
        SDL_SetRenderDrawColor(rend, color, color, color, 255);
        color -= 50;
        arm_pos += animation::forward_kinematics(j);
        SDL_RenderDrawLineF(rend, prev_pos.x, prev_pos.y, arm_pos.x, arm_pos.y);
        prev_pos = arm_pos;

        if (!j->child) break;
        j = j->child->child;
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *win = SDL_CreateWindow("Arm",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        600, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(win, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event evt;

    auto shoulder = std::make_unique<animation::Joint>(glm::vec2(0.f, 0.f), -M_PI/3.f);
    auto elbow = std::make_unique<animation::Joint>(glm::vec2(100.f, 0.f), M_PI/3.f);
    auto wrist = std::make_unique<animation::Joint>(glm::vec2(80.f,0.f), M_PI/6.f);
    auto end = std::make_unique<animation::Joint>(glm::vec2(40.f, 0.f), 0.f);

    auto b12 = std::make_unique<animation::Bone>(shoulder.get(), elbow.get());
    auto b23 = std::make_unique<animation::Bone>(elbow.get(), wrist.get());
    auto b34 = std::make_unique<animation::Bone>(wrist.get(), end.get());

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(rend);

        render(rend, shoulder.get());

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}

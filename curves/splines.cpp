#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <glm/glm.hpp>
#include <array>
#include <vector>

enum class SplineType
{
    BSPLINE,
    BEZIER
};

glm::vec2 bspline_p_t(float t, const std::vector<glm::vec2> &control, int n)
{
    std::array<float, 4> basis_fns = {
        std::pow(1 - t, 3.f) / 6.f,
        (3.f*std::pow(t, 3.f) - 6.f*std::pow(t, 2.f) + 4.f) / 6.f,
        (-3.f*std::pow(t, 3.f) + 3.f*std::pow(t, 2.f) + 3.f*t + 1.f) / 6.f,
        std::pow(t, 3.f) / 6.f
    };

    glm::vec2 p(0.f);
    for (int i = 0; i < 4; ++i)
        p += control[n + i] * basis_fns[i];
    return p;
}

glm::vec2 bernstein_p_t(float t, const std::vector<glm::vec2> &control, int n)
{
    std::array<float, 4> basis_fns = {
        std::pow(1.f - t, 3.f),
        3.f * t * std::pow(1.f - t, 2.f),
        3.f * t * t * (1.f - t),
        std::pow(t, 3.f)
    };

    glm::vec2 p(0.f);
    for (int i = 0; i < 4; ++i)
        p += control[n + i] * basis_fns[i];
    return p;
}

void draw_bspline(SDL_Renderer *rend, const std::vector<glm::vec2> &control, SplineType type)
{
    auto fn = type == SplineType::BSPLINE ? bspline_p_t : bernstein_p_t;
    int inc = type == SplineType::BSPLINE ? 1 : 3;

    for (size_t n = 0; n <= control.size() - 4; n += inc)
    {
        glm::vec2 prev = fn(0.f, control, n);
        for (float t = .001f; t <= 1.f; t += .001f)
        {
            glm::vec2 p = fn(t, control, n);
            SDL_RenderDrawLineF(rend, prev.x, prev.y, p.x, p.y);
            prev = p;
        }
    }
}

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

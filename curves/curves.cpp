#include "curves.h"
#include <array>

namespace curves
{
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
}

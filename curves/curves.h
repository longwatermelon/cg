#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

namespace curves
{
    enum class SplineType
    {
        BSPLINE,
        BEZIER
    };

    glm::vec2 bspline_p_t(float t, const std::vector<glm::vec2> &control, int n);
    glm::vec2 bernstein_p_t(float t, const std::vector<glm::vec2> &control, int n);
    void draw_bspline(SDL_Renderer *rend, const std::vector<glm::vec2> &control, SplineType type);
}

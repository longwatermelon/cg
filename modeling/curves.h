#pragma once
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>
#include <array>

namespace curves
{
    enum class SplineType
    {
        BSPLINE,
        BEZIER
    };

    float bspline_p_t(float t, const std::vector<float> &control, int start_index);
    float bernstein_p_t(float t, const std::vector<float> &control, int start_index);
    void draw_bspline(SDL_Renderer *rend, const std::vector<glm::vec2> &control, SplineType type);

    float bicubic_bezier_p_t(float u, float v, const std::array<std::array<glm::vec3, 4>, 4> &mp);
}

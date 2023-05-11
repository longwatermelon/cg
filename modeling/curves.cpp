#include "curves.h"
#include <array>

namespace curves
{
    float bspline_p_t(float t, const std::vector<float> &control, int start_index)
    {
        std::array<float, 4> basis_fns = {
            std::pow(1 - t, 3.f) / 6.f,
            (3.f*std::pow(t, 3.f) - 6.f*std::pow(t, 2.f) + 4.f) / 6.f,
            (-3.f*std::pow(t, 3.f) + 3.f*std::pow(t, 2.f) + 3.f*t + 1.f) / 6.f,
            std::pow(t, 3.f) / 6.f
        };

        float p = 0.f;
        for (int i = 0; i < 4; ++i)
            p += control[start_index + i] * basis_fns[i];
        return p;
    }

    float bernstein_p_t(float t, const std::vector<float> &control, int start_index)
    {
        std::array<float, 4> basis_fns = {
            std::pow(1.f - t, 3.f),
            3.f * t * std::pow(1.f - t, 2.f),
            3.f * t * t * (1.f - t),
            std::pow(t, 3.f)
        };

        float p = 0.f;
        for (int i = 0; i < 4; ++i)
            p += control[start_index + i] * basis_fns[i];
        return p;
    }

    void draw_bspline(SDL_Renderer *rend, const std::vector<glm::vec2> &control, SplineType type)
    {
        std::vector<float> vx(control.size()), vy(control.size());
        for (size_t i = 0; i < control.size(); ++i)
        {
            vx[i] = control[i].x;
            vy[i] = control[i].y;
        }

        auto fn = type == SplineType::BSPLINE ? bspline_p_t : bernstein_p_t;
        int inc = type == SplineType::BSPLINE ? 1 : 3;

        for (size_t n = 0; n <= control.size() - 4; n += inc)
        {
            glm::vec2 prev = { fn(0.f, vx, n), fn(0.f, vy, n) };
            for (float t = .001f; t <= 1.f; t += .001f)
            {
                glm::vec2 p = { fn(t, vx, n), fn(t, vy, n) };
                SDL_RenderDrawLineF(rend, prev.x, prev.y, p.x, p.y);
                prev = p;
            }
        }
    }

    float bicubic_bezier_p_t(float u, float v, const std::array<std::array<glm::vec3, 4>, 4> &mp)
    {
        std::array<std::array<float, 4>, 4> mu, mv, my;
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
            {
                mu[i][j] = mp[i][j].z;
                mv[i][j] = mp[i][j].x;
                my[i][j] = mp[i][j].y;
            }

        float P_uv = 0.f;
        for (int i = 0; i < 4; ++i) // rows
        {
            float B_i = bernstein_p_t(u, { my[i][0], my[i][1], my[i][2], my[i][3] }, 0);
            for (int j = 0; j < 4; ++j) // cols
            {
                float P_ij = my[i][j];
                float B_j = bernstein_p_t(v, { my[0][j], my[1][j], my[2][j], my[3][j] }, 0);
                P_uv += P_ij * B_i * B_j;
            }
        }

        return P_uv;
    }
}

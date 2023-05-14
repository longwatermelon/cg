#include "lighting.h"
#include "util.h"

namespace rt
{
    glm::vec3 phong(const Intersection &in, const std::vector<PointLight> &lights)
    {
        if (!in.intersects)
            return { 0.f, 0.f, 0.f };

        glm::vec3 color(0.f);
        for (const auto &light : lights)
        {
            glm::vec3 ambient = in.m->k_a;
            glm::vec3 diffuse = in.m->k_d *
                glm::dot(in.n, toD(light.pos - in.ray.along(in.t)));
            color += (light.in / (.1f * in.t * in.t + 1.f)) * (ambient + diffuse);
        }

        return color;
    }
}

#include "lighting.h"
#include "geometry.h"
#include "util.h"
#include <cstdio>

namespace rt
{
    glm::vec3 phong(const Intersection &in, const std::vector<PointLight> &lights)
    {
        if (!in.intersects)
            return { 0.f, 0.f, 0.f };

        glm::vec4 hit = in.ray.along(in.t);

        glm::vec3 total_color(0.f);
        for (const auto &light : lights)
        {
            glm::vec3 color = in.m->k_a;
            glm::vec4 l = glm::normalize(toD(light.pos - hit)); // towards light

            {
                glm::vec3 diffuse = in.m->k_d *
                    glm::dot(in.n, glm::normalize(toD(light.pos - hit)));

                glm::vec4 v = glm::normalize(toD(in.ray.o) - toD(hit)); // towards cam
                glm::vec4 r = reflect(-l, in.n);
                glm::vec3 specular = in.m->k_s *
                    std::pow(std::max(glm::dot(v, r), 0.f), in.m->q);

                color += diffuse + specular;
            }

            float distance = glm::distance(in.ray.along(in.t), light.pos);
            total_color += (light.in / (.1f * distance * distance + .5f)) *
                color;
        }

        return total_color;
    }
}

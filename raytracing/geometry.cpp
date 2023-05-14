#include "geometry.h"
#include <algorithm>
#include <cstdio>

namespace raytrace
{
    glm::vec4 todir(glm::vec4 p)
    {
        p.w = 0.f;
        return p;
    }

    glm::vec4 topoint(glm::vec4 p)
    {
        p.w = 1.f;
        return p;
    }

    glm::vec3 Scene::cast_ray(Ray r) const
    {
        if (ray_sphere_intersect(r, spheres[0], nullptr))
            return { 1.f, 0.f, 0.f };

        return {0.f, 0.f, 0.f };
    }

    bool ray_sphere_intersect(Ray r, const Sphere &sph, float *t)
    {
        r.transform(glm::inverse(sph.T));
        glm::vec4 od = todir(r.o);

        float a = glm::dot(r.d, r.d);
        float b = 2 * (glm::dot(od, r.d));
        float c = glm::dot(od, od) - sph.r * sph.r;
        float d = b * b - 4.f * a * c;

        if (d < 0.f)
            return false;

        float t1 = (-b - std::sqrt(d)) / (2.f * a);
        float t2 = (-b + std::sqrt(d)) / (2.f * a);

        if (t1 < 0 && t2 < 0) return false;

        if (t) *t = std::min(t1, t2);
        return true;
    }
}

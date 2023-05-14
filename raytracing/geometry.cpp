#include "geometry.h"
#include <algorithm>
#include <cstdio>
#include <glm/gtx/string_cast.hpp>

namespace raytrace
{
    glm::vec4 toD(glm::vec4 p)
    {
        p.w = 0.f;
        return p;
    }

    glm::vec4 toP(glm::vec4 p)
    {
        p.w = 1.f;
        return p;
    }

    glm::vec4 toD(glm::vec3 p)
    {
        return { p.x, p.y, p.z, 0.f };
    }

    glm::vec4 toP(glm::vec3 p)
    {
        return { p.x, p.y, p.z, 1.f };
    }

    glm::vec3 to3(glm::vec4 p)
    {
        return { p.x, p.y, p.z };
    }

    glm::vec3 Scene::cast_ray(Ray r) const
    {
        if (spheres[0].ray_intersect(r, nullptr))
            return { 1.f, 0.f, 0.f };

        return {0.f, 0.f, 0.f };
    }

    bool Sphere::ray_intersect(Ray r, float *t) const
    {
        r.transform(glm::inverse(this->T));
        glm::vec4 od = toD(r.o);

        float a = glm::dot(r.d, r.d);
        float b = 2 * (glm::dot(od, r.d));
        float c = glm::dot(od, od) - this->r * this->r;
        float d = b * b - 4.f * a * c;

        if (d < 0.f)
            return false;

        float t1 = (-b - std::sqrt(d)) / (2.f * a);
        float t2 = (-b + std::sqrt(d)) / (2.f * a);

        // No intersection
        if (t1 < 0 && t2 < 0) return false;

        // Inside sphere, one intersection
        if (t1 <= 0.f) return t2;
        if (t2 <= 0.f) return t1;

        // Two intersections
        if (t) *t = std::min(t1, t2);
        return true;
    }

    bool Triangle::ray_intersect(Ray r, float *t, glm::vec3 *bary) const
    {
        r.transform(glm::inverse(this->T));

        glm::vec3 v = to3(this->verts[0].pos - r.o);
        glm::vec3 a_b = to3(this->verts[0].pos - this->verts[1].pos);
        glm::vec3 a_c = to3(this->verts[0].pos - this->verts[2].pos);
        glm::mat3 m = glm::transpose(glm::mat3{
            a_b.x, a_c.x, r.d.x,
            a_b.y, a_c.y, r.d.y,
            a_b.z, a_c.z, r.d.z
        });

        glm::vec3 byt = glm::inverse(m) * v;

        if (byt[0] + byt[1] <= 1.f && byt[0] >= 0.f && byt[1] >= 0.f)
        {
            if (t) *t = byt[2];
            if (bary) *bary = { 1.f - byt[0] - byt[1], byt[0], byt[1] };
            return true;
        }

        return false;
    }
}

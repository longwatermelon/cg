#include "geometry.h"
#include <algorithm>
#include <cstdio>
#include <glm/gtx/string_cast.hpp>

namespace rt
{
    Intersection Sphere::ray_intersect(Ray r) const
    {
        r.transform(glm::inverse(this->T));
        glm::vec4 od = toD(r.o);

        float a = glm::dot(r.d, r.d);
        float b = 2.f * glm::dot(od, r.d);
        float c = glm::dot(od, od) - this->r * this->r;
        float d = b * b - 4.f * a * c;

        if (d < 0.f)
            return Intersection{ .intersects = false };

        float t1 = (-b + std::sqrt(d)) / (2.f * a);
        float t2 = (-b - std::sqrt(d)) / (2.f * a);

        float t = INFINITY;

        if (t1 <= 1e-4f)
            return Intersection{ .intersects = false };

        if (t2 <= 1e-4f)
            t = t1;
        else
            t = std::min(t1, t2);

        glm::vec4 n = toD(glm::normalize(r.along(t)));
        r.transform(this->T);
        n = glm::normalize(toD(glm::transpose(glm::inverse(this->T)) * n));

        return Intersection{
            .intersects = true,
            .ray = r,
            .t = t,
            .n = n,
            .m = &this->m
        };
    }

    Intersection Triangle::ray_intersect(Ray r) const
    {
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
            return Intersection{
                .intersects = true,
                .ray = r,
                .t = byt[2],
                .n = toD(glm::normalize(glm::cross(-a_b, -a_c))),
                .has_bary = true,
                .bary = { 1.f - byt[0] - byt[1], byt[0], byt[1] }
            };
        }

        return Intersection{
            .intersects = false
        };
    }

    Intersection Mesh::ray_intersect(Ray r) const
    {
        r.transform(glm::inverse(this->T));
        Intersection nearest{ .intersects = false, .t = INFINITY };
        for (const auto &tri : this->tris)
        {
            Intersection in = tri.ray_intersect(r);
            if (in.intersects && in.t < nearest.t)
                nearest = in;
        }

        nearest.ray.transform(this->T);
        nearest.m = &this->m;
        return nearest;
    }

    Intersection Plane::ray_intersect(Ray r) const
    {
        float denom = glm::dot(-this->n, r.d);
        if (denom > 0.f)
        {
            return Intersection{
                .intersects = true,
                .ray = r,
                .t = -glm::dot(toD(this->p0) - toD(r.o), this->n) / denom,
                .n = this->n,
                .m = &this->m
            };
        }

        return Intersection{
            .intersects = false
        };
    }

    glm::vec4 reflect(glm::vec4 in, glm::vec4 n)
    {
        return glm::normalize(in - 2 * glm::dot(in, n) * n);
    }
}

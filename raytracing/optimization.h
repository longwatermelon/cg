#pragma once
#include "util.h"
#include <glm/glm.hpp>

namespace rt
{
    struct Mesh;

    struct AABB
    {
        bool ray_intersect(Ray r) const;
        static AABB create(const Mesh &m);

        glm::vec3 min, max;
    };
}

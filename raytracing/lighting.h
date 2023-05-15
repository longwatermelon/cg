#pragma once
#include "util.h"
#include "scene.h"
#include <glm/glm.hpp>
#include <vector>

namespace rt
{
    struct PointLight
    {
        glm::vec4 pos;
        float in;
    };

    glm::vec3 phong(const Intersection &in, const Scene &sc);
}

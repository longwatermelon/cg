#pragma once
#include "util.h"
#include "scene.h"
#include <glm/glm.hpp>
#include <vector>
#include <functional>

namespace rt
{
    struct PointLight
    {
        glm::vec4 pos;
        float in;
    };

    glm::vec3 phong(const Intersection &in, const Scene &sc);
    glm::vec3 reflect_color(const Intersection &in, const Scene &sc, glm::vec3 obj_col,
        const std::function<glm::vec3(const Intersection&, const Scene&)> &lighting_fn);
    glm::vec3 refract_color(const Intersection &in, const Scene &sc,
        const std::function<glm::vec3(const Intersection&, const Scene&)> &lighting_fn,
        int counter = 0);
    bool check_shadowed(const Intersection &in, const Scene &sc, const PointLight &l);
}

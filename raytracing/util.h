#pragma once
#include <glm/glm.hpp>

namespace rt
{
    // convert point to direction (homogeneous coords)
    glm::vec4 toD(glm::vec4 p);
    // convert direction to point (homogeneous coords)
    glm::vec4 toP(glm::vec4 p);

    glm::vec4 toD(glm::vec3 p);
    glm::vec4 toP(glm::vec3 p);

    glm::vec3 to3(glm::vec4 p);

    struct Ray
    {
        void transform(glm::mat4 T)
        {
            o = T * o;
            d = T * d;
        }

        glm::vec4 along(float t) const
        {
            return o + t * d;
        }

        glm::vec4 o, d;
    };

    struct Material
    {
        glm::vec3 k_a, k_d, k_s;
    };

    struct Intersection
    {
        bool intersects;
        Ray ray;
        float t;
        glm::vec4 n;
        const Material *m;

        bool has_bary{ false };
        glm::vec3 bary;
    };
}

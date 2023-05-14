#include "util.h"

namespace rt
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
}

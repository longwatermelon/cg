#include "util.h"
#include <glm/gtx/quaternion.hpp>

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

    glm::mat4 rotation(glm::vec3 rot)
    {
        glm::quat qy(glm::vec3(0.f, rot.y, 0.f));
        glm::quat qx(glm::vec3(rot.x, 0.f, 0.f));
        glm::quat qz(glm::vec3(0.f, 0.f, rot.z));
        return glm::mat4_cast(glm::normalize(qx * qy * qz));
    }

    glm::vec4 make_doublesided(glm::vec4 n, glm::vec4 rd)
    {
        // Flip if same direction
        return glm::dot(n, rd) > 0.f ? -n : n;
    }
}

#include "animation.h"

namespace animation
{
    glm::mat3 get_transform(const Joint *j)
    {
        glm::mat3 translation = {
            1.f, 0.f, j->pos.x,
            0.f, 1.f, j->pos.y,
            0.f, 0.f, 1.f
        };

        glm::mat3 rotation = {
            std::cos(j->angle), -std::sin(j->angle), 0.f,
            std::sin(j->angle), std::cos(j->angle), 0.f,
            0.f, 0.f, 1.f
        };

        glm::mat3 m = translation * rotation;

        if (j->parent) return get_transform(j->parent->parent) * m;
        else return m;
    }

    glm::vec2 forward_kinematics(const Joint *j)
    {
        if (!j->parent)
            return j->pos;
        return get_transform(j->parent->parent) * glm::vec3(j->pos.x, j->pos.y, 1.f);
    }
}

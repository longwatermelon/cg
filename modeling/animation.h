#pragma once
#include <memory>
#include <glm/glm.hpp>

namespace animation
{
    struct Bone;

    struct Joint
    {
        Joint(glm::vec2 pos, float angle)
            : pos(pos), angle(angle) {}
        Bone *parent{ nullptr }, *child{ nullptr };

        glm::vec2 pos;
        float angle{ 0.f };
    };

    struct Bone
    {
        Bone(Joint *parent, Joint *child)
            : parent(parent), child(child)
        {
            parent->child = this;
            child->parent = this;
        }
        Joint *parent{ nullptr }, *child{ nullptr };
    };


    glm::mat3 get_transform(const Joint *j);
    glm::vec2 forward_kinematics(const Joint *j);
}

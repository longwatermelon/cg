#pragma once
#include <glm/glm.hpp>
#include <opencv2/opencv.hpp>
#include <array>

namespace rt
{
    // convert point to direction (homogeneous coords)
    glm::vec4 toD(glm::vec4 p);
    // convert direction to point (homogeneous coords)
    glm::vec4 toP(glm::vec4 p);

    glm::vec4 toD(glm::vec3 p);
    glm::vec4 toP(glm::vec3 p);

    glm::vec3 to3(glm::vec4 p);

    glm::mat4 rotation(glm::vec3 rot);

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

        glm::vec4 o{ toP(glm::vec3{ 0.f }) }, d{ 0.f };
    };

    struct Material
    {
        glm::vec3 k_a, k_d, k_s;
        float q;
        float reflectiveness{ 0.f };
        float refractiveness{ 0.f };
        float refract_n{ 1.f };

        bool textured{ false }, has_norm_map{ false };
        cv::Mat texture, norm_map;
    };

    struct Vertex;

    struct Intersection
    {
        bool intersects{ false };
        Ray ray;
        float t{ 0.f };
        glm::vec4 n{ 0.f };
        const Material *m{ nullptr };

        bool has_bary{ false };
        glm::vec3 bary{ 0.f };
        std::array<const Vertex*, 3> verts;
    };
}

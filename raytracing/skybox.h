#pragma once
#include "util.h"
#include <array>
#include <opencv2/opencv.hpp>
#include <glm/glm.hpp>

namespace rt
{
    struct Skybox
    {
        glm::vec3 ray_intersect(glm::vec4 dir);
        // -x, +x, -y, +y, -z, +z
        std::array<cv::Mat, 6> images;
        glm::ivec2 dim;
    };
}

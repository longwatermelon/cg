#include "skybox.h"
#include <cmath>

static int sign(float f) { return f < 0 ? -1 : 1; }

namespace rt
{
    glm::vec3 Skybox::ray_intersect(glm::vec4 dir)
    {
        // Find longest r.d dimension magnitude
        // -x = 0, +x = 1, -y = 2, +y = 3, -z = 4, +z = 5
        // Skybox cube is 1x1x1, ray is in the center
        // Change in longest dimension = 0.5 * sign
        // Assuming x = longest dimension:
        // dx/r.d.x = dy/r.d.y = dz/r.d.z
        char longest = 'x';
        glm::vec4 adir = glm::abs(dir);
        if (adir.x > adir.y && adir.x > adir.z) longest = 'x';
        else if (adir.y > adir.x && adir.y > adir.z) longest = 'y';
        else if (adir.z > adir.y && adir.z > adir.x) longest = 'z';

        int u, v;
        int index;

        if (longest == 'x')
        {
            float dx = .5f * sign(dir.x);
            float dy = dx / dir.x * dir.y;
            float dz = dx / dir.x * dir.z;

            u = (dy + .5f) * this->dim.y;
            if (sign(dir.x) > 0)
            {
                v = (1.f - dz + .5f) * this->dim.x;
                index = 1;
            }
            else
            {
                v = (dz + .5f) * this->dim.x;
                index = 0;
            }
        }

        if (longest == 'z')
        {
            float dz = .5f * sign(dir.z);
            float dx = dz / dir.z * dir.x;
            float dy = dz / dir.z * dir.y;

            u = (dy + .5f) * this->dim.y;
            if (sign(dir.z > 0))
            {
                v = (dx + .5f) * this->dim.x;
                index = 5;
            }
            else
            {
                v = (1.f - dx + .5f) * this->dim.x;
                index = 4;
            }
        }

        cv::Vec3b col = this->images[index].at<cv::Vec3b>(u, v);
        return {
            (float)col.val[0] / 255.f,
            (float)col.val[1] / 255.f,
            (float)col.val[2] / 255.f
        };
    }
}

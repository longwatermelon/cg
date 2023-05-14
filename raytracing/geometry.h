#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace raytrace
{
    // convert point to direction (homogeneous coords)
    glm::vec4 todir(glm::vec4 p);
    // convert direction to point (homogeneous coords)
    glm::vec4 topoint(glm::vec4 p);

    struct Ray
    {
        Ray(glm::vec3 o, glm::vec3 d)
            : o({ o.x, o.y, o.z, 1.f }),
              d({ d.x, d.y, d.z, 0.f }) {}

        void transform(glm::mat4 T)
        {
            o = T * o;
            d = T * d;
        }

        glm::vec4 o, d;
    };

    struct Sphere
    {
        Sphere(float r)
            : r(r), T(1.f) {}
        Sphere(float r, glm::mat4 T)
            : r(r), T(T) {}

        float r;
        glm::mat4 T;
    };

    struct Scene
    {
        std::vector<Sphere> spheres;

        // Return colors all in 0 - 1 range
        glm::vec3 cast_ray(Ray r) const;
    };

    bool ray_sphere_intersect(Ray r, const Sphere &sph, float *t);
}

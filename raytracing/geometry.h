#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace raytrace
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
        Ray(glm::vec3 o, glm::vec3 d)
            : o(toP(o)),
              d(toD(d)) {}

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

        bool ray_intersect(Ray r, float *t) const;

        float r;
        glm::mat4 T;
    };

    struct Vertex
    {
        Vertex(glm::vec3 pos)
            : pos(toP(pos)) {}

        glm::vec4 pos;
    };

    struct Triangle
    {
        Triangle(std::array<Vertex, 3> verts)
            : verts(verts) {}

        bool ray_intersect(Ray r, float *t, glm::vec3 *bary) const;

        std::array<Vertex, 3> verts;
        glm::mat4 T;
    };

    struct Plane
    {
        Plane(glm::vec3 p0, glm::vec3 n)
            : p0(toP(p0)), n(toD(n)) {}

        bool ray_intersect(Ray r, float *t) const;

        glm::vec4 p0, n;
    };

    struct Scene
    {
        std::vector<Sphere> spheres;
        std::vector<Triangle> triangles;
        std::vector<Plane> planes;

        // Return colors all in 0 - 1 range
        glm::vec3 cast_ray(Ray r) const;
    };
}

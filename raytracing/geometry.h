#pragma once
#include "util.h"
#include <glm/glm.hpp>
#include <vector>
#include <array>

namespace rt
{
    struct Sphere
    {
        Intersection ray_intersect(Ray r) const;

        float r;
        glm::mat4 T;
        Material m;
    };

    struct Vertex
    {
        glm::vec4 pos;
    };

    struct Triangle
    {
        Intersection ray_intersect(Ray r) const;

        std::array<Vertex, 3> verts;
        glm::mat4 T;
    };

    struct Mesh
    {
        Intersection ray_intersect(Ray r) const;

        std::vector<Triangle> tris;
        Material m;
    };

    struct Plane
    {
        Intersection ray_intersect(Ray r) const;

        glm::vec4 p0, n;
        Material m;
    };

    glm::vec4 reflect(glm::vec4 in, glm::vec4 n);
}

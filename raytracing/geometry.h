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
    };

    struct Mesh
    {
        Intersection ray_intersect(Ray r) const;

        std::vector<Triangle> tris;
        Material m;
    };

    struct Model
    {
        Intersection ray_intersect(Ray r) const;

        std::vector<Mesh> meshes;
        glm::mat4 T;
    };

    struct Plane
    {
        Intersection ray_intersect(Ray r) const;

        glm::vec4 p0, n;
        Material m;
    };

    // in is away from camera
    glm::vec4 reflect(glm::vec4 in, glm::vec4 n);
    // n_r = n_i / n_t
    // in is towards camera
    glm::vec4 refract(glm::vec4 in, glm::vec4 n, float n_r);
}

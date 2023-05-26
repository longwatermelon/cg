#pragma once
#include "util.h"
#include "optimization.h"
#include <glm/glm.hpp>
#include <vector>
#include <array>
#include <string>

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
        glm::vec4 pos, norm;
        glm::vec2 tc;
    };

    struct Mesh
    {
        Intersection ray_intersect(Ray r) const;
        Intersection ray_intersect_tri(Ray r, std::array<const Vertex*, 3> verts) const;

        std::array<Mesh, 8> split_into8() const;

        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        Material m;

        AABB bounding_box{
            glm::vec3(-INFINITY), glm::vec3(INFINITY)
        };
    };

    struct Model
    {
        Intersection ray_intersect(Ray r) const;
        void generate_mesh_aabb();

        void load_meshes(const std::string &path);
        void split_into8();

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

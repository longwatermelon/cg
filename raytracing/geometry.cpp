#include "geometry.h"
#include "optimization.h"
#include "util.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

#define NO_INTERSECTION Intersection{ .intersects = false, .ray = r, .t = INFINITY }

const float EPSILON = 1e-2f;

namespace rt
{
    Intersection Sphere::ray_intersect(Ray r) const
    {
        r.transform(glm::inverse(this->T));
        glm::vec4 od = toD(r.o);

        float a = glm::dot(r.d, r.d);
        float b = 2.f * glm::dot(od, r.d);
        float c = glm::dot(od, od) - this->r * this->r;
        float d = b * b - 4.f * a * c;

        if (d < 0.f)
            return NO_INTERSECTION;
        float t1 = (-b + std::sqrt(d)) / (2.f * a);
        float t2 = (-b - std::sqrt(d)) / (2.f * a);

        float t = INFINITY;

        if (t1 <= 1e-4f)
            return NO_INTERSECTION;

        if (t2 <= 1e-4f)
            t = t1;
        else
            t = std::min(t1, t2);

        glm::vec4 n = toD(glm::normalize(r.along(t)));
        r.transform(this->T);
        n = glm::normalize(toD(glm::transpose(glm::inverse(this->T)) * n));
        if (glm::dot(n, r.d) > 0.f)
            n = -n;

        return Intersection{
            .intersects = true,
            .ray = r,
            .t = t,
            .n = n,
            .m = &this->m
        };
    }

    Intersection Mesh::ray_intersect(Ray r) const
    {
        if (!this->bounding_box.ray_intersect(r))
            return NO_INTERSECTION;

        Intersection nearest = NO_INTERSECTION;
        for (size_t i = 0; i < this->indices.size(); i += 3)
        {
            Intersection in = ray_intersect_tri(r,
                {
                    &this->verts[i],
                    &this->verts[i + 1],
                    &this->verts[i + 2]
                });

            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        nearest.m = &this->m;
        return nearest;
    }

    Intersection Mesh::ray_intersect_tri(Ray r, std::array<const Vertex*, 3> verts) const
    {
        glm::vec3 v = to3(verts[0]->pos - r.o);
        glm::vec3 a_b = to3(verts[0]->pos - verts[1]->pos);
        glm::vec3 a_c = to3(verts[0]->pos - verts[2]->pos);
        glm::mat3 m = glm::transpose(glm::mat3{
            a_b.x, a_c.x, r.d.x,
            a_b.y, a_c.y, r.d.y,
            a_b.z, a_c.z, r.d.z
        });

        glm::vec3 byt = glm::inverse(m) * v;

        if (byt[0] + byt[1] <= 1.f && byt[0] >= 0.f && byt[1] >= 0.f)
        {
            if (byt[2] < 0.f)
                return NO_INTERSECTION;

            glm::vec3 bary = { 1.f - byt[0] - byt[1], byt[0], byt[1] };
            glm::vec4 n = bary[0] * verts[0]->norm +
                          bary[1] * verts[1]->norm +
                          bary[2] * verts[2]->norm;

            if (glm::dot(n, r.d) > 0.f)
                n = -n;

            return Intersection{
                .intersects = true,
                .ray = r,
                .t = byt[2],
                .n = n,
                .has_bary = true,
                .bary = bary,
                .verts = verts
            };
        }

        return NO_INTERSECTION;
    }

    std::array<Mesh, 8> Mesh::split_into8() const
    {
        glm::vec3 average_pos(0.f);
        for (const auto &v : this->verts)
            average_pos += to3(v.pos);
        average_pos /= this->verts.size();

        // 0 = -x -y -z
        // 1 = +x -y -z
        // 2 = +x -y +z
        // 3 = -x -y +z

        // 4 = -x +y -z
        // 5 = +x +y -z
        // 6 = +x +y +z
        // 7 = -x +y +z
        std::array<Mesh, 8> meshes;
        for (auto &m : meshes)
            m.m = this->m;

        for (unsigned int i = 0; i < this->indices.size(); i += 3)
        {
            std::array<Vertex, 3> v = {
                this->verts[this->indices[i]],
                this->verts[this->indices[i + 1]],
                this->verts[this->indices[i + 2]]
            };

            for (auto &vert : v)
            {
                std::vector<unsigned int> indexes;
                glm::vec3 p = to3(vert.pos - toP(average_pos));
                if (p.x < 0 && p.y < 0 && p.z < 0) indexes.emplace_back(0);
                if (p.x >= 0 && p.y < 0 && p.z < 0) indexes.emplace_back(1);
                if (p.x >= 0 && p.y < 0 && p.z >= 0) indexes.emplace_back(2);
                if (p.x < 0 && p.y < 0 && p.z >= 0) indexes.emplace_back(3);

                if (p.x < 0 && p.y >= 0 && p.z < 0) indexes.emplace_back(4);
                if (p.x >= 0 && p.y >= 0 && p.z < 0) indexes.emplace_back(5);
                if (p.x >= 0 && p.y >= 0 && p.z >= 0) indexes.emplace_back(6);
                if (p.x < 0 && p.y >= 0 && p.z >= 0) indexes.emplace_back(7);

                for (unsigned int index : indexes)
                {
                    for (int j = 0; j < 3; ++j)
                    {
                        meshes[index].verts.emplace_back(v[j]);
                        meshes[index].indices.emplace_back(meshes[index].verts.size() - 1);
                    }
                }
            }
        }

        return meshes;
    }

    Intersection Model::ray_intersect(Ray r) const
    {
        r.transform(glm::inverse(this->T));
        Intersection nearest = NO_INTERSECTION;
        for (const auto &mesh : this->meshes)
        {
            Intersection in = mesh.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        nearest.ray.transform(this->T);
        nearest.n = glm::normalize(
            toD(glm::transpose(
                glm::inverse(this->T)
            ) * nearest.n)
        );

        if (nearest.t < EPSILON)
            return NO_INTERSECTION;

        return nearest;
    }

    void Model::generate_mesh_aabb()
    {
        for (auto &mesh : this->meshes)
            mesh.bounding_box = AABB::create(mesh);
    }

    void Model::split_into8()
    {
        std::vector<Mesh> meshes;
        for (const auto &mesh : this->meshes)
        {
            std::array<Mesh, 8> submeshes = mesh.split_into8();
            for (const auto &mesh : submeshes)
                meshes.emplace_back(mesh);
        }

        this->meshes = meshes;
    }

    void Model::load_meshes(const std::string &path)
    {
        std::ifstream ifs(path);
        std::string first;

        std::vector<glm::vec4> pos, norms;
        std::vector<glm::vec2> tcs;

        for (std::string line; std::getline(ifs, line);)
        {
            std::stringstream ss(line);
            ss >> first;

            if (first == "o")
            {
                this->meshes.emplace_back(Mesh{
                    .m = Material{
                        .k_a = { .2f, 0.f, 0.f },
                        .k_d = { .5f, 0.f, 0.f },
                        .k_s = { .7f, 0.f, 0.f },
                        .q = 50.f
                    }
                });
            }

            if (first == "v")
            {
                glm::vec4 p;
                ss >> p.x >> p.y >> p.z;
                pos.emplace_back(toP(p));
            }

            if (first == "vn")
            {
                glm::vec4 n;
                ss >> n.x >> n.y >> n.z;
                norms.emplace_back(toD(n));
            }

            if (first == "vt")
            {
                glm::vec2 tc;
                ss >> tc.x >> tc.y;
                tcs.emplace_back(tc);
            }

            if (first == "f")
            {
                char c;
                for (int i = 0; i < 3; ++i)
                {
                    int ip, it = -1, in;
                    ss >> ip >> c;
                    if (ss.peek() == '/') ss >> c;
                    else ss >> it >> c;
                    ss >> in;

                    this->meshes.back().verts.emplace_back(Vertex{
                        .pos = pos[ip - 1],
                        .norm = norms[in - 1],
                        .tc = it > 0 ? tcs[it - 1] : glm::vec2(0.f)
                    });
                    this->meshes.back().indices.emplace_back(
                        this->meshes.back().verts.size() - 1
                    );
                }
            }
        }
    }

    Intersection Plane::ray_intersect(Ray r) const
    {
        float denom = glm::dot(-this->n, r.d);
        if (denom > 0.f)
        {
            return Intersection{
                .intersects = true,
                .ray = r,
                .t = -glm::dot(toD(this->p0) - toD(r.o), this->n) / denom,
                .n = this->n,
                .m = &this->m
            };
        }

        return NO_INTERSECTION;
    }

    glm::vec4 reflect(glm::vec4 in, glm::vec4 n)
    {
        return glm::normalize(in - 2 * glm::dot(in, n) * n);
    }

    glm::vec4 refract(glm::vec4 in, glm::vec4 n, float n_r)
    {
        return (
            n_r * glm::dot(in, n) -
            std::sqrt(1.f - n_r * n_r *
                (1.f - std::pow(glm::dot(in, n), 2.f)))
        ) * n - n_r * in;
    }
}

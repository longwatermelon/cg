#include "geometry.h"
#include "optimization.h"
#include "util.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <glm/gtx/string_cast.hpp>

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
            return Intersection{ .intersects = false };

        float t1 = (-b + std::sqrt(d)) / (2.f * a);
        float t2 = (-b - std::sqrt(d)) / (2.f * a);

        float t = INFINITY;

        if (t1 <= 1e-4f)
            return Intersection{ .intersects = false };

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

    Intersection Mesh::ray_intersect(Ray r, bool smooth_shading) const
    {
        if (!this->bounding_box.ray_intersect(r))
            return Intersection{ .intersects = false };

        Intersection nearest{ .intersects = false, .t = INFINITY };
        for (unsigned int i = 0; i < this->indices.size(); i += 3)
        {
            Intersection in = ray_intersect_tri(r,
                {
                    this->verts[i],
                    this->verts[i + 1],
                    this->verts[i + 2]
                }, smooth_shading);

            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        nearest.m = &this->m;
        return nearest;
    }

    Intersection Mesh::ray_intersect_tri(Ray r, std::array<Vertex, 3> verts,
                                    bool smooth_shading) const
    {
        glm::vec3 v = to3(verts[0].pos - r.o);
        glm::vec3 a_b = to3(verts[0].pos - verts[1].pos);
        glm::vec3 a_c = to3(verts[0].pos - verts[2].pos);
        glm::mat3 m = glm::transpose(glm::mat3{
            a_b.x, a_c.x, r.d.x,
            a_b.y, a_c.y, r.d.y,
            a_b.z, a_c.z, r.d.z
        });

        glm::vec3 byt = glm::inverse(m) * v;

        if (byt[0] + byt[1] <= 1.f && byt[0] >= 0.f && byt[1] >= 0.f)
        {
            if (byt[2] < 0.f)
                return Intersection{ .intersects = false };

            glm::vec4 n;
            glm::vec3 bary = { 1.f - byt[0] - byt[1], byt[0], byt[1] };
            if (smooth_shading)
            {
                n = bary[0] * verts[0].norm +
                    bary[1] * verts[1].norm +
                    bary[2] * verts[2].norm;
            }
            else
            {
                n = toD(glm::normalize(glm::cross(-a_b, -a_c)));
            }

            if (glm::dot(n, r.d) > 0.f)
                n = -n;

            return Intersection{
                .intersects = true,
                .ray = r,
                .t = byt[2],
                .n = n,
                .has_bary = true,
                .bary = bary
            };
        }

        return Intersection{
            .intersects = false
        };
    }

    Intersection Model::ray_intersect(Ray r, bool smooth_shading) const
    {
        r.transform(glm::inverse(this->T));
        Intersection nearest{ .intersects = false, .t = INFINITY };
        for (const auto &mesh : this->meshes)
        {
            Intersection in = mesh.ray_intersect(r, smooth_shading);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        nearest.ray.transform(this->T);
        nearest.n = glm::normalize(
            toD(glm::transpose(
                glm::inverse(this->T)
            ) * nearest.n)
        );

        return nearest;
    }

    void Model::generate_mesh_aabb()
    {
        for (auto &mesh : this->meshes)
            mesh.bounding_box = AABB::create(mesh);
    }

    void Model::load_meshes(const std::string &path)
    {
        std::ifstream ifs(path);
        std::string first;

        std::vector<glm::vec4> pos, norms;

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
                pos.emplace_back(p);
            }

            if (first == "vn")
            {
                glm::vec4 n;
                ss >> n.x >> n.y >> n.z;
                norms.emplace_back(n);
            }

            if (first == "f")
            {
                char c;
                int int_;
                for (int i = 0; i < 3; ++i)
                {
                    int ip, in;
                    ss >> ip >> c;
                    if (ss.peek() == '/') ss >> c;
                    else ss >> int_ >> c;
                    ss >> in;

                    this->meshes.back().verts.emplace_back(Vertex{
                        .pos = pos[ip - 1],
                        .norm = norms[in - 1]
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

        return Intersection{
            .intersects = false
        };
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

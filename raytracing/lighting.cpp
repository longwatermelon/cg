#include "lighting.h"
#include "geometry.h"
#include "util.h"
#include "scene.h"
#include <cstdio>

static const float EPSILON = 1e-5f;

namespace rt
{
    glm::vec3 phong(Intersection in, const Scene &sc)
    {
        if (!in.intersects)
            return sc.skybox ? sc.skybox->ray_intersect(in.ray.d) : glm::vec3{ 0.f, 0.f, 0.f };

        glm::vec4 hit = in.ray.along(in.t);

        glm::vec3 total_color(0.f);
        for (auto light : sc.lights)
        {
            glm::vec3 color = in.m->k_a;
            glm::vec4 l = glm::normalize(toD(light.pos - hit)); // towards light

            glm::vec3 d = in.m->k_d;
            glm::vec3 s = in.m->k_s;

            if (in.m->has_norm_map && in.has_bary)
            {
                glm::vec2 uv = in.bary[0] * in.verts[0]->tc +
                               in.bary[1] * in.verts[1]->tc +
                               in.bary[2] * in.verts[2]->tc;
                uv *= in.m->texscale;
                uv.x = std::fmod(uv.x, 1.f);
                uv.y = std::fmod(uv.y, 1.f);
                cv::Vec3b col = in.m->norm_map.at<cv::Vec3b>(
                    uv.y * in.m->norm_map.rows,
                    uv.x * in.m->norm_map.cols
                );
                glm::vec3 norm = {
                    (float)col.val[2] / 255.f,
                    (float)col.val[1] / 255.f,
                    (float)col.val[0] / 255.f
                };
                norm = glm::normalize(norm * 2.f - 1.f);
                norm.z = -norm.z;
                norm.y = -norm.y;
                in.n = toD(norm);
            }

            if (in.m->textured && in.has_bary)
            {
                glm::vec2 uv = in.bary[0] * in.verts[0]->tc +
                               in.bary[1] * in.verts[1]->tc +
                               in.bary[2] * in.verts[2]->tc;
                uv *= in.m->texscale;
                uv.x = std::fmod(uv.x, 1.f);
                uv.y = std::fmod(uv.y, 1.f);

                cv::Vec3b col = in.m->texture.at<cv::Vec3b>(
                    uv.y * in.m->texture.rows,
                    uv.x * in.m->texture.cols
                );
                glm::vec3 tex = {
                    (float)col.val[2] / 255.f,
                    (float)col.val[1] / 255.f,
                    (float)col.val[0] / 255.f
                };
                d *= tex;
                s *= tex;
                color *= tex;
            }

            // Color
            if (!check_shadowed(in, sc, light))
            {
                glm::vec3 diffuse = d *
                    std::max(glm::dot(in.n, glm::normalize(toD(light.pos - hit))), 0.f);

                glm::vec4 v = glm::normalize(toD(in.ray.o) - toD(hit)); // towards cam
                glm::vec4 r = reflect(-l, in.n);
                glm::vec3 specular = s *
                    std::pow(std::max(glm::dot(v, r), 0.f), in.m->q);

                color += diffuse + specular;
            }

            // Reflection and refraction
            if (in.m->reflectiveness > 0.f)
                color = reflect_color(in, sc, color, phong);

            if (in.m->refract_n > 1.f)
                color = in.m->refractiveness * refract_color(in, sc, phong) +
                        (1.f - in.m->refractiveness) * color;

            float distance = glm::distance(in.ray.along(in.t), light.pos);
            total_color += (light.in / (.1f * distance * distance + .5f)) *
                color;
        }

        return total_color;
    }

    glm::vec3 reflect_color(const Intersection &in, const Scene &sc,
                            glm::vec3 obj_col,
        const std::function<glm::vec3(const Intersection&, const Scene&)>
            &lighting_fn)
    {
        glm::vec4 reflect_dir = reflect(in.ray.d, in.n);
        return (1.f - in.m->reflectiveness) * obj_col +
                in.m->reflectiveness * lighting_fn(sc.cast_ray(Ray{
            .o = in.ray.along(in.t) + EPSILON * in.n,
            .d = reflect_dir
        }, SC_NO_TRANSFORM_CAM), sc);
    }

    glm::vec3 refract_color(const Intersection &in, const Scene &sc,
        const std::function<glm::vec3(const Intersection&, const Scene&)>
            &lighting_fn, int counter)
    {
        if (!in.intersects || counter > 4)
            return sc.skybox ? sc.skybox->ray_intersect(in.ray.d) : glm::vec3{ 0.f, 0.f, 0.f };

        if (in.m->refract_n <= 1.f)
            return lighting_fn(in, sc);

        // Assume no objects inside other objects
        float n_r = glm::dot(in.ray.d, in.n) <= 1.f ?
            1.f / in.m->refract_n :
            in.m->refract_n;
        glm::vec4 refract_dir = refract(-in.ray.d, in.n, n_r);
        Ray ray{
            .o = in.ray.along(in.t) + EPSILON * -in.n,
            .d = refract_dir
        };
        Intersection next_in = sc.cast_ray(ray, SC_NO_TRANSFORM_CAM);
        return refract_color(next_in, sc, lighting_fn, counter + 1);
    }

    bool check_shadowed(const Intersection &in, const Scene &sc, const PointLight &l)
    {
        Ray sray;
        sray.o = in.ray.along(in.t) + EPSILON * in.n;
        sray.d = glm::normalize(toD(l.pos - sray.o));
        Intersection in_s = sc.cast_ray(sray, SC_NO_TRANSFORM_CAM);

        return in_s.intersects && in_s.t < glm::length(to3(l.pos - sray.o)) && in_s.t > 0.f;
    }
}

#include "scene.h"
#include "geometry.h"
#include "util.h"
#include "lighting.h"
#include "raytrace.h"
#include <nlohmann/json.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <fstream>

using namespace nlohmann;

namespace rt
{
    Intersection Scene::cast_ray(Ray r, SceneRayOpts opts) const
    {
        if (!(opts & SC_NO_TRANSFORM_CAM))
            r.transform(this->cam);

        Intersection nearest{ .intersects = false, .ray = r, .t = INFINITY };

        for (const auto &sph : this->spheres)
        {
            Intersection in = sph.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        for (const auto &model : this->models)
        {
            Intersection in = model.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        for (const auto &plane : this->planes)
        {
            Intersection in = plane.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        nearest.ray = r;
        return nearest;
    }

    Scene Scene::from(const std::string &src)
    {
        Scene sc;

        std::ifstream ifs(src);
        json j = json::parse(ifs);

        if (j.contains("image") && j["image"].contains("width"))
            setw(j["image"]["width"].get<float>());
        if (j.contains("image") && j["image"].contains("height"))
            seth(j["image"]["height"].get<float>());

        if (j.contains("camera"))
        {
            if (j["camera"].contains("pos"))
                sc.cam = glm::translate(glm::mat4(1.f), {
                    j["camera"]["pos"][0], j["camera"]["pos"][1], j["camera"]["pos"][2]
                });
            if (j["camera"].contains("rotation"))
                sc.cam *= rotation({
                    j["camera"]["rotation"][0], j["camera"]["rotation"][1], j["camera"]["rotation"][2]
                });
        }

        if (j.contains("skybox"))
        {
            sc.skybox = std::make_unique<Skybox>();
            sc.skybox->images = {
                // -x, +x, -y, +y, -z, +z
                cv::imread(j["skybox"]["-x"]),
                cv::imread(j["skybox"]["+x"]),
                cv::imread(j["skybox"]["-y"]),
                cv::imread(j["skybox"]["+y"]),
                cv::imread(j["skybox"]["-z"]),
                cv::imread(j["skybox"]["+z"])
            };
            sc.skybox->dim = {
                sc.skybox->images[0].cols,
                sc.skybox->images[0].rows
            };
        }

        if (j.contains("materials"))
        {
            for (auto &mat : j["materials"])
            {
                Material m;
                if (mat.contains("color"))
                {
                    glm::vec3 color = {
                        mat["color"][0],
                        mat["color"][1],
                        mat["color"][2]
                    };
                    m.k_a = mat["ambient"].get<float>() * color;
                    m.k_d = mat["diffuse"].get<float>() * color;
                    m.k_s = mat["specular"].get<float>() * color;
                }

                if (mat.contains("specular exponent"))
                    m.q = mat["specular exponent"];
                if (mat.contains("reflect"))
                    m.reflectiveness = mat["reflect"];
                if (mat.contains("n"))
                    m.refract_n = mat["n"];
                if (mat.contains("refract"))
                    m.refractiveness = mat["refract"];
                if (mat.contains("texture"))
                {
                    m.textured = true;
                    m.texture = cv::imread(mat["texture"]);
                }

                sc.materials.emplace_back(mat["name"], m);
            }
        }
        else
        {
            std::cout << "**WARNING**: No materials detected.\n";
        }

        if (j.contains("objects"))
        {
            for (auto &obj : j["objects"])
            {
                if (obj["type"] == "sphere")
                {
                    Sphere sph;
                    sph.r = obj["radius"];
                    sph.m = *sc.find_material(obj["material"].get<std::string>());
                    sph.T = glm::translate(glm::mat4(1.f), {
                        obj["pos"][0], obj["pos"][1], obj["pos"][2]
                    });

                    if (obj.contains("rotation"))
                        sph.T *= rotation({
                            obj["rotation"][0], obj["rotation"][1], obj["rotation"][2]
                        });

                    if (obj.contains("stretch"))
                        sph.T = sph.T * glm::transpose(glm::mat4{
                            obj["stretch"][0].get<float>(), 0.f, 0.f, 0.f,
                            0.f, obj["stretch"][1].get<float>(), 0.f, 0.f,
                            0.f, 0.f, obj["stretch"][2].get<float>(), 0.f,
                            0.f, 0.f, 0.f, 1.f
                        });
                    sc.spheres.emplace_back(sph);
                }

                if (obj["type"] == "model")
                {
                    Model m;
                    m.load_meshes(obj["path"]);
                    m.T = glm::translate(glm::mat4(1.f), {
                        obj["pos"][0], obj["pos"][1], obj["pos"][2]
                    });

                    if (obj.contains("rotation"))
                        m.T *= rotation({
                            obj["rotation"][0], obj["rotation"][1], obj["rotation"][2]
                        });

                    for (auto &mesh : m.meshes)
                        mesh.m = *sc.find_material(obj["material"].get<std::string>());

                    if (obj.contains("subdivide"))
                    {
                        for (int i = 0; i < obj["subdivide"]; ++i)
                            m.split_into8();
                    }

                    m.generate_mesh_aabb();

                    sc.models.emplace_back(m);
                }

                if (obj["type"] == "plane")
                {
                    Plane p;
                    p.p0 = toP({
                        obj["p0"][0], obj["p0"][1], obj["p0"][2]
                    });
                    p.n = toD({
                        obj["normal"][0], obj["normal"][1], obj["normal"][2]
                    });
                    p.m = *sc.find_material(obj["material"].get<std::string>());
                    sc.planes.emplace_back(p);
                }
            }
        }
        else
        {
            std::cout << "**WARNING**: No objects detected.\n";
        }

        if (j.contains("lights"))
        {
            for (auto &light : j["lights"])
            {
                PointLight l;
                l.in = light["intensity"];
                l.pos = toP({ light["pos"][0], light["pos"][1], light["pos"][2] });
                sc.lights.emplace_back(l);
            }
        }
        else
        {
            std::cout << "**WARNING**: No lights detected.\n";
        }

        if (j.contains("post"))
        {
            for (auto &cmd : j["post"])
                sc.post_commands.emplace_back(cmd);
        }

/*         for (const auto &v : sc.models[0].meshes[0].verts) */
/*             printf("%f %f %f | %f %f %f | %f %f\n", v.pos.x, v.pos.y, v.pos.z, */
/*                     v.norm.x, v.norm.y, v.norm.z, v.tc.x, v.tc.y); */

        return sc;
    }

    const Material *Scene::find_material(const std::string &name) const
    {
        for (const auto &pair : this->materials)
        {
            if (pair.first == name)
                return &pair.second;
        }

        return nullptr;
    }
}

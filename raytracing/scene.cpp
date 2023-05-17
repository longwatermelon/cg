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

        Intersection nearest{ .intersects = false, .t = INFINITY };

        for (const auto &sph : this->spheres)
        {
            Intersection in = sph.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        for (const auto &model : this->models)
        {
            Intersection in = model.ray_intersect(r, opts & SC_INTERP_NORMALS);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        for (const auto &plane : this->planes)
        {
            Intersection in = plane.ray_intersect(r);
            if (in.intersects && in.t < nearest.t && in.t > 0.f)
                nearest = in;
        }

        return nearest;
    }

    Scene Scene::from(const std::string &src)
    {
        Scene sc;

        std::ifstream ifs(src);
        json j = json::parse(ifs);

        setw(j["image"]["width"].get<float>());
        seth(j["image"]["height"].get<float>());

        sc.cam = glm::translate(glm::mat4(1.f), {
            j["camera"]["pos"][0], j["camera"]["pos"][1], j["camera"]["pos"][2]
        }) * rotation({
            j["camera"]["rotation"][0], j["camera"]["rotation"][1], j["camera"]["rotation"][2]
        });

        for (auto &mat : j["materials"])
        {
            Material m;
            glm::vec3 color = {
                mat["color"][0],
                mat["color"][1],
                mat["color"][2]
            };
            m.k_a = mat["ambient"].get<float>() * color;
            m.k_d = mat["diffuse"].get<float>() * color;
            m.k_s = mat["specular"].get<float>() * color;
            m.q = mat["specular exponent"];
            if (mat.contains("reflect"))
                m.reflectiveness = mat["reflect"];
            if (mat.contains("n"))
                m.refract_n = mat["n"];
            sc.materials.emplace_back(mat["name"], m);
        }

        for (auto &obj : j["objects"])
        {
            if (obj["type"] == "sphere")
            {
                Sphere sph;
                sph.r = obj["radius"];
                sph.m = *sc.find_material(obj["material"].get<std::string>());
                sph.T = glm::translate(glm::mat4(1.f), {
                    obj["pos"][0], obj["pos"][1], obj["pos"][2]
                }) * rotation({
                    obj["rotation"][0], obj["rotation"][1], obj["rotation"][2]
                });
                sc.spheres.emplace_back(sph);
            }

            if (obj["type"] == "model")
            {
                Model m;
                m.load_meshes(obj["path"]);
                m.generate_mesh_aabb();
                m.T = glm::translate(glm::mat4(1.f), {
                    obj["pos"][0], obj["pos"][1], obj["pos"][2]
                }) * rotation({
                    obj["rotation"][0], obj["rotation"][1], obj["rotation"][2]
                });
                for (auto &mesh : m.meshes)
                    mesh.m = *sc.find_material(obj["material"].get<std::string>());
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

        for (auto &light : j["lights"])
        {
            PointLight l;
            l.in = light["intensity"];
            l.pos = toP({ light["pos"][0], light["pos"][1], light["pos"][2] });
            sc.lights.emplace_back(l);
        }

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

#pragma once
#include "util.h"
#include "skybox.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

namespace rt
{
    struct Sphere;
    struct Model;
    struct Plane;
    struct PointLight;

    enum SceneRayOpts
    {
        SC_NO_TRANSFORM_CAM = 1
    };

    struct Scene
    {
        glm::mat4 cam{ glm::mat4(1.f) };
        std::vector<Sphere> spheres;
        std::vector<Model> models;
        std::vector<Plane> planes;
        std::vector<PointLight> lights;
        std::vector<std::pair<std::string, Material>> materials;
        std::unique_ptr<Skybox> skybox;
        std::vector<std::string> post_commands;

        Intersection cast_ray(Ray r, SceneRayOpts opts = (SceneRayOpts)0) const;

        static Scene from(const std::string &src);

        const Material *find_material(const std::string &name) const;
    };
}

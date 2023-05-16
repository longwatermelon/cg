#pragma once
#include "util.h"
#include <glm/glm.hpp>
#include <vector>

namespace rt
{
    struct Sphere;
    struct Model;
    struct Plane;
    struct PointLight;

    enum SceneRayOpts
    {
        SC_NO_TRANSFORM_CAM = 1,
        SC_INTERP_NORMALS = 2
    };

    struct Scene
    {
        glm::mat4 cam{ glm::mat4(1.f) };
        std::vector<Sphere> spheres;
        std::vector<Model> models;
        std::vector<Plane> planes;
        std::vector<PointLight> lights;

        Intersection cast_ray(Ray r, SceneRayOpts opts = (SceneRayOpts)0) const;
    };
}

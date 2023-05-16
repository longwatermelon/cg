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
        SC_NO_TRANSFORM_CAM = 1
    };

    struct Scene
    {
        glm::mat4 cam;
        std::vector<Sphere> spheres;
        std::vector<Model> models;
        std::vector<Plane> planes;
        std::vector<PointLight> lights;

        Intersection cast_ray(Ray r, SceneRayOpts opts = (SceneRayOpts)0) const;
    };
}

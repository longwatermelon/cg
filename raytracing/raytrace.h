#pragma once
#include "geometry.h"
#include "lighting.h"
#include <string>

namespace rt
{
    struct Scene
    {
        std::vector<Sphere> spheres;
        std::vector<Mesh> meshes;
        std::vector<Plane> planes;
        std::vector<PointLight> lights;

        Intersection cast_ray(Ray r) const;
    };

    void render(const Scene &sc, const std::string &outf);
}

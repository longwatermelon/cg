#include "scene.h"
#include "geometry.h"

namespace rt
{
    Intersection Scene::cast_ray(Ray r, bool cam_transform) const
    {
        if (cam_transform)
            r.transform(this->cam);

        Intersection nearest{ .intersects = false, .t = INFINITY };

        for (const auto &sph : this->spheres)
        {
            Intersection in = sph.ray_intersect(r);
            if (in.intersects && in.t < nearest.t)
                nearest = in;
        }

        for (const auto &mesh : this->meshes)
        {
            Intersection in = mesh.ray_intersect(r);
            if (in.intersects && in.t < nearest.t)
                nearest = in;
        }

        for (const auto &plane : this->planes)
        {
            Intersection in = plane.ray_intersect(r);
            if (in.intersects && in.t < nearest.t)
                nearest = in;
        }

        return nearest;
    }
}

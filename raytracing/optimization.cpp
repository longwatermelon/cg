#include "optimization.h"
#include "geometry.h"
#include <cmath>

namespace rt
{
    bool AABB::ray_intersect(Ray r) const
    {
        // ro.? + t * rd.? = ?min
        // t = (?min - ro.?) / rd.?
        float tx1, tx2,
              ty1, ty2,
              tz1, tz2;
        tx1 = (this->min.x - r.o.x) / r.d.x;
        tx2 = (this->max.x - r.o.x) / r.d.x;
        ty1 = (this->min.y - r.o.y) / r.d.y;
        ty2 = (this->max.y - r.o.y) / r.d.y;
        tz1 = (this->min.z - r.o.z) / r.d.z;
        tz2 = (this->max.z - r.o.z) / r.d.z;
        return std::max(std::max(std::min(tx1, tx2), std::min(ty1, ty2)), std::min(tz1, tz2)) <=
               std::min(std::min(std::max(tx1, tx2), std::max(ty1, ty2)), std::max(tz1, tz2));
    }

    AABB AABB::create(const Mesh &m)
    {
        AABB box{
            .min = glm::vec3(INFINITY),
            .max = glm::vec3(-INFINITY)
        };

        for (const auto &v : m.verts)
        {
            if (v.pos.x < box.min.x) box.min.x = v.pos.x;
            if (v.pos.x > box.max.x) box.max.x = v.pos.x;
            if (v.pos.y < box.min.y) box.min.y = v.pos.y;
            if (v.pos.y > box.max.y) box.max.y = v.pos.y;
            if (v.pos.z < box.min.z) box.min.z = v.pos.z;
            if (v.pos.z > box.max.z) box.max.z = v.pos.z;
        }

        return box;
    }
}

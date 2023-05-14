#include "../raytrace.h"
#include "../geometry.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char **argv)
{
    rt::Scene sc;

    rt::Material mat = {
        .k_a = { .2f, 0.f, 0.f },
        .k_d = { .5f, 0.f, 0.f },
        .k_s = { .7f, 0.f, 0.f }
    };

    sc.spheres.emplace_back(rt::Sphere{
        .r = 1.f,
        .T = glm::translate(glm::mat4(1.f), { 0.f, 1.1f, 5.f }),
        .m = mat
    });
    rt::Triangle tri{
        .verts = {
            rt::Vertex{ .pos = rt::toP({ -1.f, -1.f, 0.f }) },
            rt::Vertex{ .pos = rt::toP({ -1.f, 1.f, 0.f }) },
            rt::Vertex{ .pos = rt::toP({ 1.f, -1.f, 0.f }) }
        },
        .T = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 4.5f })
    };

    rt::Mesh mesh{
        .tris = {
            tri
        },
        .m = mat
    };
    sc.meshes.emplace_back(mesh);

    sc.planes.emplace_back(rt::Plane{
        .p0 = rt::toP({ 0.f, 1.f, 5.f }),
        .n = rt::toD({ 0.f, -1.f, 0.f }),
        .m = mat
    });

    sc.lights.emplace_back(rt::PointLight{
        .pos = rt::toP({ 2.f, -2.f, 0.f }),
        .in = .5f
    });

    rt::render(sc, "out.ppm");

    return 0;
}

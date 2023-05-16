#include "../raytrace.h"
#include "../geometry.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

int main(int argc, char **argv)
{
    rt::Scene sc;
    sc.cam = glm::translate(glm::mat4(1.f), { 5.f, 0.f, -3.f })
             * rt::rotation({ 0.f, -0.5f, 0.f });

    rt::Material mat = {
        .k_a = { .2f, .2f, .2f },
        .k_d = { .5f, .5f, .5f },
        .k_s = { .7f, .7f, .7f },
        .q = 50.f,
        .reflectiveness = .6f
    };

    rt::Material mat2 = {
        .k_a = { .2f, .2f, 0.f },
        .k_d = { .3f, .5f, 0.f },
        .k_s = { .5f, .7f, 0.f },
        .q = 100.f
    };

    rt::Material mat3 = {
        .k_a = { 0.f, .2f, .3f },
        .k_d = { 0.f, .5f, .7f },
        .k_s = { 0.f, .7f, .5f },
        .q = 10.f
    };

    rt::Material mat4 = {
        .k_a = { 0.f, .2f, .3f },
        .k_d = { 0.f, .5f, .7f },
        .k_s = { 0.f, .7f, .5f },
        .q = 10.f
    };

    sc.spheres.emplace_back(rt::Sphere{
        .r = 1.f,
        .T = glm::translate(glm::mat4(1.f), { 0.f, 1.f, 5.f }) *
            glm::transpose(glm::mat4{
                2.f, 0.f, 0.f, 0.f,
                0.f, 1.f, 0.f, 0.f,
                0.f, 0.f, 2.f, 0.f,
                0.f, 0.f, 0.f, 1.f
            }),
        .m = mat2
    });

    sc.spheres.emplace_back(rt::Sphere{
        .r = 1.f,
        .T = glm::translate(glm::mat4(1.f), { 2.2f, -1.f, 8.f }),
        .m = mat4
    });

    rt::Mesh mesh{
        .tris = {
            rt::Triangle{
                .verts = {
                    rt::Vertex{ .pos = rt::toP({ 1.f, -1.f, 0.f }) },
                    rt::Vertex{ .pos = rt::toP({ -1.f, -1.f, 0.f }) },
                    rt::Vertex{ .pos = rt::toP({ -1.f, 1.f, 0.f }) }
                }
            },
            rt::Triangle{
                .verts = {
                    rt::Vertex{ .pos = rt::toP({ 1.1f, -0.9f, 0.f }) },
                    rt::Vertex{ .pos = rt::toP({ -0.9f, 1.1f, 0.f }) },
                    rt::Vertex{ .pos = rt::toP({ 1.1f, 1.1f, 0.f }) }
                }
            }
        },
        .m = mat3,
        .T = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 4.5f })
    };
    sc.meshes.emplace_back(mesh);

    sc.planes.emplace_back(rt::Plane{
        .p0 = rt::toP({ 0.f, 1.f, 5.f }),
        .n = rt::toD(glm::normalize(glm::vec3{ 0.f, -1.f, 0.f })),
        .m = mat
    });

    sc.lights.emplace_back(rt::PointLight{
        .pos = rt::toP({ 1.f, 0.f, 3.f }),
        .in = 1.f
    });

    rt::render(sc, "out.ppm");

    return 0;
}

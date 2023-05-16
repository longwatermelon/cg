#include "../raytrace.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char **argv)
{
    rt::Scene sc;

    rt::Model model;
    model.load_meshes("res/bunny.obj");
    model.generate_mesh_aabb();
    model.T = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 5.f });
    sc.models.emplace_back(model);
    printf("Model loaded\n");

    sc.lights.emplace_back(rt::PointLight{
        .pos = rt::toP({ 0.f, 0.f, 0.f }),
        .in = 2.f
    });

    rt::render(sc, "out.ppm", rt::RenderConfig{
        .render_opts = rt::RENDER_LOG_PROGRESS,
        .scene_opts = rt::SC_INTERP_NORMALS
    });

    return 0;
}

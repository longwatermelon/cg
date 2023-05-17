#include "../raytrace.h"

int main()
{
    rt::Scene sc = rt::Scene::from("scenes/test.json");
    rt::render(sc, "out.ppm", rt::RenderConfig{
        .render_opts = rt::RENDER_LOG_PROGRESS,
        .scene_opts = rt::SC_INTERP_NORMALS
    });
    return 0;
}

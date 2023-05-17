#include "../raytrace.h"
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc == 1)
    {
        printf("No file provided.\n");
        exit(EXIT_FAILURE);
    }

    rt::Scene sc = rt::Scene::from(argv[1]);
    // rt::phong(sc.cast_ray(rt::Ray{
    //     .o = rt::toP({ 0.f, 0.f, 0.f }),
    //     .d = rt::toD({ 0.f, 0.f, 1.f })
    // }, rt::SC_INTERP_NORMALS), sc);
    rt::render(sc, "out.ppm", rt::RenderConfig{
        .render_opts = (rt::RenderOptions)(rt::RENDER_LOG_PROGRESS | rt::RENDER_ANTIALIASING),
        .scene_opts = rt::SC_INTERP_NORMALS
    });
    return 0;
}

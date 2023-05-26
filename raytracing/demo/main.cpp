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
    rt::render(sc, "out.ppm", rt::RenderConfig{
        .render_opts = (rt::RenderOptions)(rt::RENDER_LOG_PROGRESS | rt::RENDER_ANTIALIASING)
    });
    return 0;
}

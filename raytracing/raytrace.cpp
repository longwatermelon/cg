#include "raytrace.h"
#include "lighting.h"
#include "scene.h"
#include <array>
#include <fstream>

static int WIDTH = 1000;
static int HEIGHT = 1000;

namespace rt
{
    void render(const Scene &sc, const std::string &outf,
                RenderConfig cfg)
    {
        std::vector<glm::vec3> frame(WIDTH * HEIGHT);
        for (int y = 0; y < HEIGHT; ++y)
        {
            if (cfg.render_opts & RENDER_LOG_PROGRESS)
            {
                printf("\r%d rows rendered", y);
                fflush(stdout);
            }

            for (int x = 0; x < WIDTH; ++x)
            {
                float fx = cfg.render_opts & RENDER_ANTIALIASING ? (float)(x - .5f) : x;
                float fy = cfg.render_opts & RENDER_ANTIALIASING ? (float)(y - .5f) : y;

                float th = (fx / WIDTH) - .5f;
                float phi = (fy / HEIGHT) - .5f;
                Ray r{
                    .o = toP({ 0.f, 0.f, 0.f }),
                    .d = toD(glm::normalize(
                                glm::vec3{ std::sin(th), std::sin(phi), 1.f }))
                };

                frame[y * WIDTH + x] = phong(
                    sc.cast_ray(r, cfg.scene_opts), sc
                );
            }
        }

        if (cfg.render_opts & RENDER_LOG_PROGRESS)
            printf("\r%d rows rendered\n", HEIGHT);

        if (cfg.render_opts & RENDER_ANTIALIASING)
        {
            if (cfg.render_opts & RENDER_LOG_PROGRESS)
                printf("Antialiasing...\n");

            for (int y = 0; y < HEIGHT - 1; ++y)
            {
                for (int x = 0; x < WIDTH - 1; ++x)
                {
                    frame[y * WIDTH + x] = (frame[y * WIDTH + x] +
                                   frame[(y + 1) * WIDTH + x] +
                                   frame[y * WIDTH + x + 1] +
                                   frame[(y + 1) * WIDTH + x + 1]) / 4.f;
                }
            }
        }

        if (cfg.render_opts & RENDER_LOG_PROGRESS)
            printf("Writing to file...\n");

        std::ofstream ofs(outf);
        ofs << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

        for (int i = 0; i < WIDTH * HEIGHT; ++i)
        {
            ofs <<
                (int)std::abs(255.f * std::min(std::max(frame[i].r, 0.f), 1.f)) << ' ' <<
                (int)std::abs(255.f * std::min(std::max(frame[i].g, 0.f), 1.f)) << ' ' <<
                (int)std::abs(255.f * std::min(std::max(frame[i].b, 0.f), 1.f)) <<
                '\n';
        }

        ofs.close();

        for (const auto &cmd : sc.post_commands)
            std::system(cmd.c_str());
    }

    void setw(int w)
    {
        WIDTH = w;
    }

    void seth(int h)
    {
        HEIGHT = h;
    }
}

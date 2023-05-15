#include "raytrace.h"
#include "lighting.h"
#include <array>
#include <fstream>

#define WIDTH 1000
#define HEIGHT 1000

namespace rt
{
    void render(const Scene &sc, const std::string &outf)
    {
        std::vector<glm::vec3> frame(WIDTH * HEIGHT);
        for (int y = 0; y < HEIGHT; ++y)
        {
            for (int x = 0; x < WIDTH; ++x)
            {
                float th = ((float)x / WIDTH) - .5f;
                float phi = ((float)y / HEIGHT) - .5f;
                Ray r{
                    .o = toP({ 0.f, 0.f, 0.f }),
                    .d =
                    toD(glm::normalize(glm::vec3{ std::sin(th), std::sin(phi), 1.f }))
                };

                frame[y * WIDTH + x] = phong(sc.cast_ray(r), sc);
            }
        }

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
    }
}

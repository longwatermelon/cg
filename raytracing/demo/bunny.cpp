#include "../raytrace.h"
#include "../loadobj.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char **argv)
{
    rt::Scene sc;
    sc.cam = glm::mat4(1.f);
    sc.models.emplace_back(rt::objload::load("res/plane.obj"));
    sc.models[0].T = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 5.f }) *
                     rt::rotation({ 0.f, M_PI, M_PI });
    printf("Model loaded\n");

    sc.lights.emplace_back(rt::PointLight{
        .pos = rt::toP({ 0.f, 0.f, 0.f }),
        .in = 1.f
    });

    rt::render(sc, "out.ppm",
        rt::RENDER_LOG_PROGRESS, rt::SC_INTERP_NORMALS);

    return 0;
}

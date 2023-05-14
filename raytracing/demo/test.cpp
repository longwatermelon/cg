#include "../raytrace.h"
#include "../geometry.h"
#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char **argv)
{
    raytrace::Scene sc;
    sc.spheres.emplace_back(1.f,
        glm::translate(glm::mat4(1.f), { 0.f, 0.f, 5.f }));
    // raytrace::Triangle tri({
    //     raytrace::Vertex({ -1.f, -1.f, 0.f }),
    //     raytrace::Vertex({ 1.f, -1.f, 0.f }),
    //     raytrace::Vertex({ -1.f, 1.f, 0.f })
    // });
    // tri.T = glm::translate(glm::mat4(1.f), { 0.f, 0.f, 5.f });

    // sc.triangles.emplace_back(tri);

    raytrace::render(sc, "out.ppm");

    return 0;
}

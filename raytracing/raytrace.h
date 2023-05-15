#pragma once
#include "scene.h"
#include "geometry.h"
#include "lighting.h"
#include <string>

namespace rt
{
    void render(const Scene &sc, const std::string &outf);
}

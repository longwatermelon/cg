#pragma once
#include "scene.h"
#include "geometry.h"
#include "lighting.h"
#include <string>

namespace rt
{
    enum RenderOptions
    {
        RENDER_LOG_PROGRESS = 1
    };

    void render(const Scene &sc, const std::string &outf,
                RenderOptions ropt = (RenderOptions)0,
                SceneRayOpts scopt = (SceneRayOpts)0);
}

#pragma once
#include "scene.h"
#include "geometry.h"
#include "lighting.h"
#include <string>

namespace rt
{
    enum RenderOptions
    {
        RENDER_LOG_PROGRESS = 1,
        RENDER_ANTIALIASING = 2
    };

    struct RenderConfig
    {
        RenderOptions render_opts = (RenderOptions)0;
        SceneRayOpts scene_opts = (SceneRayOpts)0;
    };

    void render(const Scene &sc, const std::string &outf,
            RenderConfig cfg = RenderConfig());

    void setw(int w);
    void seth(int h);
}

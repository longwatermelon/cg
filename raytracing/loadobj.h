#pragma once
#include "geometry.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace rt::objload
{
    Model load(const std::string &path);
    void process_node(Model &model, aiNode *node, const aiScene *sc);
    Mesh process_mesh(aiMesh *mesh, const aiScene *sc);
}

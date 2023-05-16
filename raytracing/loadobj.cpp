#include "loadobj.h"
#include "util.h"

namespace rt::objload
{
    Model load(const std::string &path)
    {
        Model m;

        Assimp::Importer im;
        const aiScene *scene = im.ReadFile(path, aiProcess_Triangulate/* | aiProcess_FlipUVs */);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            fprintf(stderr, "[rt::objload::load] Assimp error when loading model.\n");
            exit(EXIT_FAILURE);
        }

        process_node(m, scene->mRootNode, scene);
        return m;
    }

    void process_node(Model &model, aiNode *node, const aiScene *sc)
    {
        for (size_t i = 0; i < node->mNumMeshes; ++i)
        {
            struct aiMesh *mesh = sc->mMeshes[node->mMeshes[i]];
            model.meshes.emplace_back(process_mesh(mesh, sc));
        }

        for (size_t i = 0; i < node->mNumChildren; ++i)
        {
            process_node(model, node->mChildren[i], sc);
        }
    }

    Mesh process_mesh(aiMesh *mesh, const aiScene *sc)
    {
        std::vector<Vertex> verts;
        std::vector<unsigned int> indices;
        // std::vector<Texture*> textures;

        // vertices
        for (size_t i = 0; i < mesh->mNumVertices; ++i)
        {
            Vertex v;
            v.pos.x = mesh->mVertices[i].x;
            v.pos.y = mesh->mVertices[i].y;
            v.pos.z = mesh->mVertices[i].z;
            v.pos = toP(v.pos);

            v.norm.x = mesh->mNormals[i].x;
            v.norm.y = mesh->mNormals[i].y;
            v.norm.z = mesh->mNormals[i].z;
            v.norm = toD(v.norm);

            // if (mesh->mTextureCoords[0])
            // {
            //     v.tex_coords.x = mesh->mTextureCoords[0][i].x;
            //     v.tex_coords.y = mesh->mTextureCoords[0][i].y;
            // }
            // else
            // {
            //     v.tex_coords = glm::vec2(0.f, 0.f);
            // }

            verts.emplace_back(v);
        }

        // indices
        for (size_t i = 0; i < mesh->mNumFaces; ++i)
        {
            aiFace face = mesh->mFaces[i];

            for (size_t j = 0; j < face.mNumIndices; ++j)
                indices.emplace_back(face.mIndices[j]);
        }

        return Mesh{
            .verts = verts,
            .indices = indices,
            .m = Material{
                .k_a = { .2f, .2f, .2f },
                .k_d = { .5f, .5f, .5f },
                .k_s = { .7f, .7f, .7f },
                .q = 50.f
            }
        };

        // material
        // struct aiMaterial *mat = sc->mMaterials[mesh->mMaterialIndex];

        // std::vector<Texture*> diffuse = load_mat_textures(mat, aiTextureType_DIFFUSE, TextureType::DIFFUSE);
        // std::vector<Texture*> specular = load_mat_textures(mat, aiTextureType_SPECULAR, TextureType::SPECULAR);

        // for (size_t i = 0; i < diffuse.size(); ++i)
        //     textures.emplace_back(diffuse[i]);

        // for (size_t i = 0; i < specular.size(); ++i)
        //     textures.emplace_back(specular[i]);

        // return std::make_unique<Mesh>(verts, indices, textures);
    }
}

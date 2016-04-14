#pragma once

/*
RfModelGL - Class
*/

#ifndef __RF_MODEL_GL_H__
#define __RF_MODEL_GL_H__

#include <vector>

#include "rf_model.h"
#include "rf_shader.h"

#include "rf_mesh_gl.h"

namespace zootopia {

    class RfModelGL : public RfModel {

    public:

        explicit RfModelGL(const std::string& path, bool gamma = false);
        ~RfModelGL();

    public:

        void draw() override;

    private:

        void        loadModel(const std::string& path) override;
        void        processNode(aiNode* node, const aiScene* scene) override;
        
        RfMeshGL    processMesh(aiMesh* mesh, const aiScene* scene);
        GLint       textureFromFile(const char* path, std::string directory);

        std::vector<TextureGL>  loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    private:

        std::vector<TextureGL>  _texturesLoaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
        std::vector<RfMeshGL>   _meshes;
        std::string             _directory;
        bool                    _gammaCorrection;

    };

}

#endif

#pragma once

/*
RfModel - Interface
*/

#ifndef __RF_MODEL_H__
#define __RF_MODEL_H__

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "rf_shader.h"

namespace zootopia {

    class RfModel {

    public:

        RfModel() {};
        virtual ~RfModel() {};

    public:

        virtual void draw() = 0;

    protected:

        virtual void loadModel(const std::string& path) = 0;
        virtual void processNode(aiNode* node, const aiScene* scene) = 0;
    };

}

#endif
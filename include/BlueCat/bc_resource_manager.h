#pragma once

#ifndef __BC_RESOURCE_MANAGER_H__
#define __BC_RESOURCE_MANAGER_H__

#include <string>
#include <vector>

#include "rf_object.h"

namespace zootopia {

    class BcResourceManager {

    public:

        BcResourceManager();
        ~BcResourceManager();

    public:

        void initialize();

        void loadModelFromFile(const std::string &filePath);
        void loadImagesFromDir(const std::string &dirPath);

        std::vector<RfObject*> getObjects() const {
            return _objects;
        }

        void prepare();

        void deleteResources();

    private:

        void getFilesInDirectory(std::vector<std::string> &out, const std::string &directory);

        std::vector<RfObject*>  _objects;
        std::vector<RfModel*>   _models;

    };

}

#endif
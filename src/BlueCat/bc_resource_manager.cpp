#include "bc_resource_manager.h"
#include "zpd.h"

#include <iostream>
#include <string>

#include <Windows.h>

#include "rf_object.h"
#include "rf_model.h"

#include "rf_object_gl.h"
#include "rf_shader_gl.h"
#include "rf_model_gl.h"

using namespace zootopia;

BcResourceManager::BcResourceManager() {}
BcResourceManager::~BcResourceManager() {}


void BcResourceManager::initialize()
{
    //ZABORT_NOT_IMPLEMENTED();
}


void BcResourceManager::loadModelFromFile(const std::string &filePath)
{
    RfModel* testModel1 = new RfModelGL("models/cyborg/cyborg.obj");
    //RfModel* testModel2 = new RfModelGL("models/planet/planet.obj");
    RfModel* testModel2 = new RfModelGL("models/judy/judy.3ds");
    RfModel* testModel3 = new RfModelGL("models/nanosuit/nanosuit.obj");
    RfModel* testModel4 = new RfModelGL("models/floor/floor.obj");
    //RfModel* testModel4 = new RfModelGL("models/tropical/tropical.obj");
    //RfModel* testModel5 = new RfModelGL("models/sponza/sponza.obj");

    _models.push_back(testModel1);
    _models.push_back(testModel2);
    _models.push_back(testModel3);
    _models.push_back(testModel4);

    RfObject* testObject1 = new RfObjectGL(testModel1);
    testObject1->translate(RfPoint3(1, 0, 1));
    testObject1->rotate(-45, RfPoint3(0, 1, 0));

    RfObject* testObject2 = new RfObjectGL(testModel2);
    testObject2->translate(RfPoint3(-1, 0, -1));
    testObject2->scale(RfPoint3(3.0f, 3.0f, 3.0f));
    //testObject2->scale(RfPoint3(0.5f, 0.5f, 0.5f));
    testObject2->rotate(-45, RfPoint3(0, 1, 0));
    testObject2->rotate(90, RfPoint3(-1, 0, 0));
    //testObject2->rotate(45, RfPoint3(0, 1, 0));

    RfObject* testObject3 = new RfObjectGL(testModel3);
    testObject3->translate(RfPoint3(2, 0, -2));
    testObject3->scale(RfPoint3(0.3f, 0.3f, 0.3f));
    testObject3->rotate(-45, RfPoint3(0, 1, 0));

    RfObject* testObject4 = new RfObjectGL(testModel4);
    testObject4->scale(RfPoint3(5.0f, 5.0f, 5.0f));
    //testObject4->rotate(180, RfPoint3(0, 0, 1));

    //_objects.push_back(testObject1);
    //_objects.push_back(testObject2);
    //_objects.push_back(testObject3);

    _objects.push_back(testObject4);
}


void BcResourceManager::loadImagesFromDir(const std::string &dirPath)
{
    std::vector<std::string> imageFiles;
    getFilesInDirectory(imageFiles, dirPath);

    for (auto imageFile : imageFiles) {

        std::cout << imageFile << std::endl;
    }
}


void BcResourceManager::prepare()
{
    ZABORT_NOT_IMPLEMENTED();
}


void BcResourceManager::getFilesInDirectory(std::vector<std::string> &out, const std::string &directory)
{
    // TODO: Multi-platform support.
    // Currently, It is only support Windows.

    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return; /* No files found */

    do {
        const std::string file_name = file_data.cFileName;
        const std::string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        if (file_name[0] == '.')
            continue;

        if (is_directory)
            continue;

        out.push_back(full_file_name);

    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
}


void BcResourceManager::deleteResources()
{
    for (auto object : _objects)
        ZDELETEZ_SAFE(object);

    for (auto model : _models)
        ZDELETEZ_SAFE(model);
}
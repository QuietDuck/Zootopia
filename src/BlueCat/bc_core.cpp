#include "bc_core.h"
#include "zpd.h"

#include <iostream>

// GL BACKEND INCLUDE
#include "rf_compositor_gl.h"
#include "gr_window_manager_glfw.h"
#include "gr_event_manager_glfw.h"
#include "bc_resource_manager.h"

#include "rf_size.h"
#include "rf_camera.h"
#include "rf_shader.h"

#include "rf_camera_gl.h"
#include "rf_shader_gl.h"

using namespace zootopia;

static BcCore* bc_core = nullptr;

BcCore::BcCore() :
    _compositor(nullptr)
,   _windowManager(nullptr)
{}

BcCore::~BcCore() {}

// Core is single-tone pattern.
BcCore* BcCore::getInstance()
{
    if (bc_core) {
        return bc_core;
    }
    else {
        bc_core = new BcCore;
        return bc_core;
    }
}

void BcCore::terminate()
{
    if (_windowManager) {
        _windowManager->terminate();
        ZDELETEZ_SAFE(_windowManager);
    }
    if (_eventManager) {
        //_eventManager->destroy();
        ZDELETEZ_SAFE(_eventManager);
    }

    if (_resourceManager) {
        _resourceManager->deleteResources();
        ZDELETEZ_SAFE(_resourceManager);
    }

    if (_compositor) {
        _compositor->destroy();
        ZDELETEZ_SAFE(_compositor);
    }

    // DELETE ITSELF.
    if (bc_core) {
        ZDELETEZ_SAFE(bc_core);
    }
}

void BcCore::initialize(BackendType backendType)\
{
    _backendType = backendType;
    
    switch (backendType) {
        
    case BackendType::kOpenGL: {

        if (!_windowManager) {
            _windowManager = new GrWindowManagerGLFW;
        }
        if (!_eventManager) {
            _eventManager = new GrEventManagerGLFW;
        }

        if (!_resourceManager) {
            _resourceManager = new BcResourceManager;
        }

        if (!_compositor) {
            _compositor = new RfCompositorGL;
        }

        _windowManager->initialize();
        _windowManager->createWindow(RfSize(1280, 720));
        _eventManager->initialize();

        _compositor->initialize(RfSize(1280, 720));
        compositorSetUp();

        _resourceManager->initialize();
        _resourceManager->loadModelFromFile("models/cyborg/cyborg.obj");
        //_resourceManager->prepare();
        //_resourceManager->loadImagesFromDir("images");
        
        break;
    }

    case BackendType::kDirectX:

        ZABORT_NOT_IMPLEMENTED();

        break;

    default:

        ZABORT_NOT_IMPLEMENTED();

        break;
    }
}

void BcCore::run()
{
    std::vector<RfObject*> objects = _resourceManager->getObjects();

    while (!_windowManager->shouldClose()) {

        _compositor->prepareFrame(_windowManager->getWindowSize());
        _compositor->renderDisplay(objects);
        _compositor->postProcess();
        _compositor->finishFrame();

        _windowManager->swapBuffers();

        _eventManager->pollEvents();
        _eventManager->calculateDeltaTime();
    }
}

/// TEMPORARY...
void BcCore::compositorSetUp()
{
    RfCamera* initCamera = new RfCameraGL(
        glm::vec3(-3.0f, 4.0f, 3.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        -45.0f,
        -20.0f
        );

    _compositor->setCamera(initCamera);

    RfShader* initShader = new RfShaderGL("shader/glsl/deferred_shading.vert", "shader/glsl/deferred_shading.frag");

    _compositor->setShader(initShader);
}

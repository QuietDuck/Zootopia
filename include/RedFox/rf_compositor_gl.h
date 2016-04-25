#pragma once

#ifndef __RF_COMPOSITOR_GL_H__
#define __RF_COMPOSITOR_GL_H__

// STD LIBRARY
#include <vector>

// THIRD PARTY
#include <GL/glew.h>
#include <IL/il.h>

// INTERFACE, CLASS
#include "rf_compositor.h"
#include "rf_size.h"
#include "rf_shader.h"
#include "rf_camera.h"

#include "rf_light.h"
#include "rf_point_light.h"
#include "rf_dir_light.h"
#include "rf_spot_light.h"

#include "rf_light_manager_gl.h"
#include "rf_camera_gl.h"
#include "rf_grid_gl.h"
#include "rf_object_gl.h"
#include "rf_model_gl.h"
#include "rf_mesh_gl.h"
#include "rf_shader_gl.h"
#include "rf_quad_gl.h"
#include "rf_geometry_buffer_gl.h"

namespace zootopia {

    class RfCompositorGL : public RfCompositor {

        // IS THIS GOOD?
        // FOR _displayCamera, _deferredShader.
        friend class RfMeshGL;
        friend class RfObjectGL;
        friend class RfGridGL;

    public:

        RfCompositorGL();
        ~RfCompositorGL();

    public:

        void initialize(const RfSize& fboSize) override;
        void resize(const RfSize& fboSize) override;
        void destroy() override;

        void prepareFrame(const RfSize& frameSize) override;
        void finishFrame() override;

        void renderDisplay(const std::vector<RfObject*>& objects) override;
        void renderUserInterface() override;
        void renderHitTest() override;
        void readHitTestBuffer() override;

        void postProcess() override;

        void setShader(RfShader* shader) override;
        void setCamera(RfCamera* camera) override;

        static RfCameraGL* getCamera() { return _displayCamera; }

    private:

        RfGeometryBufferGL* _gBuffer;

        // framebuffer
        GLuint          _hitTestFBO;
        
        // test
        GLuint          ssbo;

        // Shader
        RfShaderGL*         _displayShader;
        RfShaderGL*         _hitTestShader;
        static RfShaderGL*  _deferredShader;

        // Camera
        static RfCameraGL*  _displayCamera;

        // Grid Helper
        RfGridGL*   _grid;

        // Quad Renderer
        RfQuadGL*   _quad;

        // Light
        RfPointLight*       _testLight;
        RfDirLight*         _testLight2;

        RfSpotLight*        _testLightR;
        RfSpotLight*        _testLightG;
        RfSpotLight*        _testLightB;

        RfLightManagerGL*   _lightManager;


    };

}

#endif
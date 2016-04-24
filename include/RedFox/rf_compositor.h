#pragma once

/*
RfCompositor - Interface
*/

#ifndef __RF_COMPOSITOR_H__
#define __RF_COMPOSITOR_H__

#include <vector>

#include "rf_object.h"
#include "rf_size.h"
#include "rf_shader.h"
#include "rf_camera.h"
#include "rf_light.h"

namespace zootopia {

    class RfCompositor {

    public:

        RfCompositor() {};
        virtual ~RfCompositor() {};

    public:

        virtual void    initialize(const RfSize& fboSize) = 0;
        virtual void    resize(const RfSize& fboSize) = 0;
        virtual void    destroy() = 0;

        virtual void    prepareFrame(const RfSize& frameSize) = 0;
        virtual void    finishFrame() = 0;

        virtual void    renderDisplay(const std::vector<RfObject*>& objects) = 0;
        virtual void    renderUserInterface() = 0;
        virtual void    renderHitTest() = 0;
        virtual void    readHitTestBuffer() = 0;

        virtual void    postProcess() = 0;

        virtual void    setShader(RfShader* shader) = 0;
        virtual void    setCamera(RfCamera* camera) = 0;
    };

}

#endif
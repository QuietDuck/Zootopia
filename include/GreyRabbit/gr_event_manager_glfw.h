#pragma once

#ifndef __BC_EVENT_MANAGER_GLFW__
#define __BC_EVENT_MANAGER_GLFW__

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gr_event_manager.h"

namespace zootopia {

    class GrEventManagerGLFW : public GrEventManager {

    public:

        GrEventManagerGLFW();
        ~GrEventManagerGLFW();

    public:

        void initialize() override;

        void pollEvents() override;
        void waitEvents() override;

        void calculateDeltaTime() override;

    };

}


#endif
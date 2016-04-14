#pragma once

#ifndef __GR_WINDOW_MANAGER_GLFW_H__
#define __GR_WINDOW_MANAGER_GLFW_H__

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gr_window_manager.h"

namespace zootopia {

    class GrWindowManagerGLFW : public GrWindowManager {

    public:

        GrWindowManagerGLFW();
        ~GrWindowManagerGLFW();

    public:

        void initialize() override;
        void createWindow(const RfSize& windowSize) override;
        void show() override;
        void swapBuffers() override;
        void terminate() override;

        int shouldClose() override;

        static GLFWwindow* getGLFWwindow() {
            return _window;
        }

    private:

        // SINGLE WINDOW.
        static GLFWwindow* _window;

    };

}

#endif

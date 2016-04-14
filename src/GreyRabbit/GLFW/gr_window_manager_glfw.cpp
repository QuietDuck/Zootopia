#include <iostream>

#include "gr_window_manager_glfw.h"
#include "zpd.h"

using namespace zootopia;

// Single Window.
GLFWwindow* GrWindowManagerGLFW::_window = nullptr;

GrWindowManagerGLFW::GrWindowManagerGLFW() {}
GrWindowManagerGLFW::~GrWindowManagerGLFW() {}

void GrWindowManagerGLFW::initialize() {

    /* Initialize the library */
    if (!glfwInit()) {
        ZLOG_E("Unable to initialize GLFW.");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_DONT_CARE);
}

void GrWindowManagerGLFW::createWindow(const RfSize& windowSize) {

    ZASSERT(windowSize.w NEQ 0);
    ZASSERT(windowSize.h NEQ 0);

    if (!_window) {

        /* Save current window size */
        _windowSize = windowSize;

        /* Create a windowed mode window and its OpenGL context */
        _window = glfwCreateWindow(
            RfScalarTruncToInt(windowSize.w),
            RfScalarTruncToInt(windowSize.h),
            _windowName.c_str(),
            nullptr, nullptr);

        if (!_window) {
            glfwTerminate();
            ZLOG_E("Failed to create GLFW window.");
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(_window);
    }
    else { // FOR EXCEPTION.

        /* If window is exist, just make current that */
        glfwMakeContextCurrent(_window);
    }
}

void GrWindowManagerGLFW::show() {

    ZABORT_NOT_IMPLEMENTED();
}

void GrWindowManagerGLFW::swapBuffers() {

    glfwSwapBuffers(_window);
}

void GrWindowManagerGLFW::terminate() {

    glfwDestroyWindow(_window);
    glfwTerminate();
}

int GrWindowManagerGLFW::shouldClose() {

    return glfwWindowShouldClose(_window);
}
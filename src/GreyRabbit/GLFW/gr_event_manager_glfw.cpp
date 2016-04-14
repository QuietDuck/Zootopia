/*
GrEventManagerGLFW - Implementation
*/
#include "gr_event_manager_glfw.h"
#include "gr_window_manager_glfw.h"
#include "zpd.h"

#include <iostream>

#include "rf_scalar.h"

#include "rf_compositor_gl.h"
#include "rf_camera_gl.h"

using namespace zootopia;

// STATIC VALUE.
static GLFWwindow* _window = nullptr;
static GLuint _currentState = GLFW_RELEASE;
static RfScalar _lastXpos = 0;
static RfScalar _lastYpos = 0;
static RfScalar  _lastFrame = 0;
static RfScalar  _deltaTime = 0;

/// TEMPORARY
static RfScalar _moveOffset = 0;

GrEventManagerGLFW::GrEventManagerGLFW() {}
GrEventManagerGLFW::~GrEventManagerGLFW() {}

/// /// /// /// /// /// ///
// EVENT CALLBACK FUNCTION
/// /// /// /// /// /// ///

///////////
// KEYBOARD
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // NOT IMPLEMENTED YET.
}


void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    // NOT IMPLEMENTED YET.
}


void charmods_callback(GLFWwindow* window, unsigned int codepoint, int mods)
{
    // NOT IMPLEMENTED YET.
}
///////////



////////
// MOUSE
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    //std::cout << xpos << ", " << ypos << std::endl;

    const RfScalar angleY = RfDoubleToScalar(xpos - _lastXpos);
    const RfScalar angleX = RfDoubleToScalar(ypos - _lastYpos);

    if (_currentState EQ GLFW_PRESS) {
        //ZLOG_I("MOUSE LEFT BUTTON PRESSING.");

        RfCamera* camera = RfCompositorGL::getCamera();
        camera->translate(RfVector3(0.0f, 0.0f, 5.0f));
        camera->rotate(angleY, RfVector3(0, 1, 0));
        camera->rotate(angleX, RfVector3(1, 0, 0));
        camera->translate(RfVector3(0.0f, 0.0f, -5.0f));
    }

    _lastXpos = RfDoubleToScalar(xpos);
    _lastYpos = RfDoubleToScalar(ypos);
}


void cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered)
    {
        // The cursor entered the client area of the window
    }
    else
    {
        // The cursor left the client area of the window
    }
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // If clicked, render hit-test.

    // HIT TEST Pseudo code.
    /*
    getCompositor
    compositor->renderHitTestBuffer();
    uint? ID = compositor->readHitTestBuffer();
    if (ID EQ 0) then getCamera or do something.
    (RfObject*)ID->onClick();
    */

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        _currentState = GLFW_PRESS;
    }
    else {
        _currentState = GLFW_RELEASE;
    }
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    // Y OFFSET IS WHEEL SCROLL.
    // X OFFSET IS HORIZONTAL WHEEL SCROLL.

    //ZLOG_I("MOUSE SCROLLING...");
    //std::cout << xoffset << ", " << yoffset << std::endl;

    const RfScalar moveScale = 0.1f;
    _moveOffset = RfDoubleToScalar(moveScale * yoffset);

    RfCamera* camera = RfCompositorGL::getCamera();
    camera->translate(RfVector3(0, 0, _moveOffset));
}


void drop_callback(GLFWwindow* window, int count, const char** paths)
{
    /*
    for (int i = 0; i < count; i++)
        handle_dropped_file(paths[i]);
    */
    ZLOG_I("DROP FILE CALLBACK");
}
////////



void GrEventManagerGLFW::initialize() {

    _window = GrWindowManagerGLFW::getGLFWwindow();

    if (_window) {
        glfwSetKeyCallback(_window, key_callback);
        glfwSetCharCallback(_window, character_callback);
        glfwSetCharModsCallback(_window, charmods_callback);
        glfwSetCursorPosCallback(_window, cursor_position_callback);
        glfwSetCursorEnterCallback(_window, cursor_enter_callback);
        glfwSetMouseButtonCallback(_window, mouse_button_callback);
        glfwSetScrollCallback(_window, scroll_callback);
        glfwSetDropCallback(_window, drop_callback);

        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    else {
        ZABORT("Event Callback can not be initialized. Window must be created first.");
    }
}


void GrEventManagerGLFW::pollEvents() {

    glfwPollEvents();
}


void GrEventManagerGLFW::waitEvents() {

    glfwWaitEvents();
}

void GrEventManagerGLFW::calculateDeltaTime()
{
    const RfScalar currentFrame = RfDoubleToScalar(glfwGetTime());
    _deltaTime = currentFrame - _lastFrame;
    _lastFrame = currentFrame;

    //std::cout << "deltaTime: " << _deltaTime << std::endl;
    //std::cout << "FPS: " << 1.0f/_deltaTime << std::endl;
}

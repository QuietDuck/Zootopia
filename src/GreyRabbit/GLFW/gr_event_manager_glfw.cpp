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
static bool _keys[GLFW_KEY_LAST];
static bool _keysPressed[GLFW_KEY_LAST];

GrEventManagerGLFW::GrEventManagerGLFW() {}
GrEventManagerGLFW::~GrEventManagerGLFW() {}

/// /// /// /// /// /// ///
// EVENT CALLBACK FUNCTION
/// /// /// /// /// /// ///

///////////
// KEYBOARD
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    RfCamera* camera = RfCompositorGL::getCamera();

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key <= GLFW_KEY_LAST)
    {
        if (action == GLFW_PRESS)
            _keys[key] = true;
        else if (action == GLFW_RELEASE)
        {
            _keys[key] = false;
            _keysPressed[key] = false;
        }
    }

    if (key == GLFW_KEY_W && action == GLFW_REPEAT)
        camera->processKeyboard(RfCamera::Movement::kForward, _deltaTime);
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        camera->processKeyboard(RfCamera::Movement::kBackward, _deltaTime);
    if (key == GLFW_KEY_A && action == GLFW_PRESS)
        camera->processKeyboard(RfCamera::Movement::kLeft, _deltaTime);
    if (key == GLFW_KEY_D && action == GLFW_PRESS)
        camera->processKeyboard(RfCamera::Movement::kRight, _deltaTime);
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


bool firstMouse = true;
////////
// MOUSE
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        _lastXpos = RfDoubleToScalar(xpos);
        _lastYpos = RfDoubleToScalar(ypos);
        firstMouse = false;
    }

    RfScalar xoffset = RfDoubleToScalar(xpos - _lastXpos);
    RfScalar yoffset = RfDoubleToScalar(_lastYpos - ypos);

    _lastXpos = RfDoubleToScalar(xpos);
    _lastYpos = RfDoubleToScalar(ypos);

    RfCamera* camera = RfCompositorGL::getCamera();
    camera->processMouseMovement(xoffset, yoffset);
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
    RfCamera* camera = RfCompositorGL::getCamera();
    camera->processMouseScroll(yoffset);
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


// TODO: Make clear.
void doMovement()
{
    RfCamera* camera = RfCompositorGL::getCamera();

    // Camera controls
    if (_keys[GLFW_KEY_W])
        camera->processKeyboard(RfCamera::Movement::kForward, _deltaTime);
    if (_keys[GLFW_KEY_S])
        camera->processKeyboard(RfCamera::Movement::kBackward, _deltaTime);
    if (_keys[GLFW_KEY_A])
        camera->processKeyboard(RfCamera::Movement::kLeft, _deltaTime);
    if (_keys[GLFW_KEY_D])
        camera->processKeyboard(RfCamera::Movement::kRight, _deltaTime);
}


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

        //glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else {
        ZABORT("Event Callback can not be initialized. Window must be created first.");
    }
}


void GrEventManagerGLFW::pollEvents() {

    glfwPollEvents();
    doMovement(); // TODO: Make Clear.
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

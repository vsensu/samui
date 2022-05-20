#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <GLFW/glfw3.h>

#include <core/core.h>
#include <core/input.h>
#include <core/engine.h>

#include "../graphics_application.h"
// clang-format on

namespace samui
{
bool Input::is_key_pressed(int keycode)
{
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    auto* window =
        static_cast<GLFWwindow*>(app->get_window().get_native_window());
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::is_mouse_button_pressed(int button)
{
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    auto* window =
        static_cast<GLFWwindow*>(app->get_window().get_native_window());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

float Input::get_mouse_x()
{
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    auto* window =
        static_cast<GLFWwindow*>(app->get_window().get_native_window());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return xpos;
}

float Input::get_mouse_y()
{
    std::shared_ptr<GraphicsApplication> app =
        std::dynamic_pointer_cast<GraphicsApplication>(
            Engine::instance().app());
    auto* window =
        static_cast<GLFWwindow*>(app->get_window().get_native_window());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return ypos;
}

}  // namespace samui

#endif
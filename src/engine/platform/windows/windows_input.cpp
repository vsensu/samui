#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <core/core.h>
#include <core/input.h>

#include <GLFW/glfw3.h>

#include <core/application.h>
// clang-format on

namespace samui {
bool Input::is_key_pressed(int keycode) {
  auto* window = static_cast<GLFWwindow*>(
      Application::instance().get_window().get_native_window());
  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::is_mouse_button_pressed(int button) {
  auto* window = static_cast<GLFWwindow*>(
      Application::instance().get_window().get_native_window());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float Input::get_mouse_x() {
  auto* window = static_cast<GLFWwindow*>(
      Application::instance().get_window().get_native_window());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return xpos;
}

float Input::get_mouse_y() {
  auto* window = static_cast<GLFWwindow*>(
      Application::instance().get_window().get_native_window());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return ypos;
}

}  // namespace samui

#endif
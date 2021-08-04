#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <core/core.h>
#include <core/input.h>

#include <GLFW/glfw3.h>

#include <core/application.h>
// clang-format on

namespace samui {
bool Input::IsKeyPressed(int keycode) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(int button) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float Input::GetMouseX() {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return xpos;
}

float Input::GetMouseY() {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return ypos;
}

}  // namespace samui

#endif
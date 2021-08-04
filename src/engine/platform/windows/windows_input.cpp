// clang-format off
#include "windows_input.h"

#include <GLFW/glfw3.h>

#include <core/application.h>
// clang-format on

namespace samui {
Input* Input::instance_ = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(int keycode) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetKey(window, keycode);
  return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(int button) {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  auto state = glfwGetMouseButton(window, button);
  return state == GLFW_PRESS;
}

float WindowsInput::GetMouseXImpl() {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return xpos;
}

float WindowsInput::GetMouseYImpl() {
  auto* window = static_cast<GLFWwindow*>(
      Application::Get().GetWindow().GetNativeWindow());
  double xpos, ypos;
  glfwGetCursorPos(window, &xpos, &ypos);
  return ypos;
}

}  // namespace samui

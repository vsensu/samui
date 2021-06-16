#include "windows_window.h"

#include <glad/glad.h>

#include "log/log.h"

namespace samui {
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

std::unique_ptr<Window> Window::Create(const WindowProps& props) {
  return std::make_unique<WindowsWindow>(props);
}

WindowsWindow::WindowsWindow(const WindowProps& props) { Init(props); }

WindowsWindow::~WindowsWindow() { Shutdown(); }

void WindowsWindow::Init(const WindowProps& props) {
  data_.Title = props.Title;
  data_.Width = props.Width;
  data_.Height = props.Height;

  SAMUI_ENGINE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width,
                    props.Height);

  if (glfwInit() != GLFW_TRUE) {
    SAMUI_ENGINE_FATAL("failed to initialize glfw");
    glfwTerminate();
    return;
  }

  window_ = glfwCreateWindow((int)props.Width, (int)props.Height,
                             data_.Title.c_str(), nullptr, nullptr);
  if (window_ == nullptr) {
    SAMUI_ENGINE_FATAL("Failed to create GLFW window");
    glfwTerminate();
    return;
  }
  glfwMakeContextCurrent(window_);
  glfwSetFramebufferSizeCallback(window_, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SAMUI_ENGINE_FATAL("Failed to initialize GLAD");
    return;
  }

  glfwSetWindowUserPointer(window_, &data_);
  SetVSync(true);
}

void WindowsWindow::Shutdown() { glfwDestroyWindow(window_); }

void WindowsWindow::OnUpdate() {
  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  glfwPollEvents();
  glfwSwapBuffers(window_);
}

void WindowsWindow::SetVSync(bool enabled) {
  if (enabled) {
    glfwSwapInterval(1);
  } else {
    glfwSwapInterval(0);
  }

  data_.VSync = enabled;
}

bool WindowsWindow::IsVSync() const { return data_.VSync; }

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}

}  // namespace samui

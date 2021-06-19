#include "windows_window.h"

#include <glad/glad.h>

#include "events/application_event.h"
#include "events/key_event.h"
#include "events/mouse_event.h"
#include "log/log.h"

namespace samui {
void glfw_error_callback(int error_code, const char* desc) {
  SAMUI_ENGINE_ERROR("GLFW Error ({0}): {1}", error_code, desc);
}

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
    glfwSetErrorCallback(&glfw_error_callback);
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

  // set glfw callbacks
  glfwSetWindowSizeCallback(
      window_, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
      });

  glfwSetWindowCloseCallback(window_, [](GLFWwindow* window) {
    WindowData&      data = *(WindowData*)glfwGetWindowUserPointer(window);
    WindowCloseEvent event;
    data.EventCallback(event);
  });

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
    switch (action) {
      case GLFW_PRESS: {
        KeyPressedEvent event(key, 0);
        data.EventCallback(event);
        break;
      }
      case GLFW_RELEASE: {
        KeyReleasedEvent event(key);
        data.EventCallback(event);
        break;
      }
      case GLFW_REPEAT: {
        KeyPressedEvent event(key, 1);
        data.EventCallback(event);
        break;
      }
    }
  });

  glfwSetMouseButtonCallback(
      window_, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        switch (action) {
          case GLFW_PRESS: {
            MouseButtonPressedEvent event(button);
            break;
          }
          case GLFW_RELEASE: {
            MouseButtonReleasedEvent event(button);
            data.EventCallback(event);
            break;
          }
        }
      });

  glfwSetScrollCallback(
      window_, [](GLFWwindow* window, double xoffset, double yoffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event(xoffset, yoffset);
        data.EventCallback(event);
      });

  glfwSetCursorPosCallback(
      window_, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData&     data = *(WindowData*)glfwGetWindowUserPointer(window);
        MouseMovedEvent event(xpos, ypos);
        data.EventCallback(event);
      });
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

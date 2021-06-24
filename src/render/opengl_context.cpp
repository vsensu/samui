#include "opengl_context.h"

#include <glad/glad.h>

#include "log/log.h"

namespace samui {

OpenGLContext::OpenGLContext(GLFWwindow* window) : window_(window) {
    SAMUI_ENGINE_ASSERT(window_, "OpenGLContext Window is null");
}

void OpenGLContext::Init() {
  glfwMakeContextCurrent(window_);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SAMUI_ENGINE_FATAL("Failed to initialize GLAD");
    return;
  }
}

void OpenGLContext::SwapBuffers() { glfwSwapBuffers(window_); }
}  // namespace samui

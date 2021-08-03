#ifdef SAMUI_RENDER_OPENGL

#include "opengl_context.h"

#include <glad/glad.h>

#include "debug/instrumentor.h"
#include "log/log.h"

namespace samui {

OpenGLContext::OpenGLContext(GLFWwindow* window) : window_(window) {
  SAMUI_ENGINE_ASSERT(window_, "OpenGLContext Window is null");
}

void OpenGLContext::Init() {
  SAMUI_PROFILE_FUNCTION();
  glfwMakeContextCurrent(window_);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    SAMUI_ENGINE_FATAL("Failed to initialize GLAD");
    return;
  }

  SAMUI_ENGINE_INFO("OpenGL info:");
  SAMUI_ENGINE_INFO("   Vendor: {0}", glGetString(GL_VENDOR));
  SAMUI_ENGINE_INFO("   Renderer: {0}", glGetString(GL_RENDERER));
  SAMUI_ENGINE_INFO("   Version: {0}", glGetString(GL_VERSION));
}

void OpenGLContext::SwapBuffers() {
  SAMUI_PROFILE_FUNCTION();
  glfwSwapBuffers(window_);
}
}  // namespace samui

#endif
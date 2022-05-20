#pragma once

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <GLFW/glfw3.h>

#include <rendering/graphics_context.h>

#include "opengl_module.h"
// clang-format on


namespace samui
{
class SAMUI_OPENGL_API OpenGLContext : public GraphicsContext
{
  public:
    OpenGLContext(GLFWwindow* window);

    void init() override;
    void swap_buffers() override;

  private:
    GLFWwindow* window_;
};
}  // namespace samui

#endif

#pragma once

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <rendering/graphics_context.h>
// clang-format on

#include <GLFW/glfw3.h>

namespace samui
{
class SAMUI_API OpenGLContext : public GraphicsContext
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

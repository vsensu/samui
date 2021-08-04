#ifndef SAMUI_OPENGL_CONTEXT_H_
#define SAMUI_OPENGL_CONTEXT_H_

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <rendering/graphics_context.h>
// clang-format on

#include <GLFW/glfw3.h>

namespace samui {
class SAMUI_API OpenGLContext : public GraphicsContext {
 public:
  OpenGLContext(GLFWwindow* window);

  void Init() override;
  void SwapBuffers() override;

 private:
  GLFWwindow* window_;
};
}  // namespace samui

#endif

#endif
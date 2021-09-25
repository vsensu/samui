// clang-format off
#include "opengl_renderer_api.h"

#include <glad/glad.h>

#include <debug/instrumentor.h>
#include "gl_errors.h"
// clang-format on

namespace samui {
void OpenGLRendererAPI::Init() {
  SAMUI_PROFILE_FUNCTION();

  initGLDebug();

  // glEnable(GL_BLEND);
  // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  // glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height) {
  glViewport(x, y, width, height);
}

void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
  glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::Clear() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::DrawIndexed(const samui::Ref<VertexArray>& vertex_array,
                                    uint32_t index_count) {
  uint32_t count = index_count == 0 ? vertex_array->GetIndexBuffer()->GetCount()
                                    : index_count;
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
  // glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRendererAPI::SetDepthTestEnable(bool enable) {
  if (enable) {
    glEnable(GL_DEPTH_TEST);
  } else {
    glDisable(GL_DEPTH_TEST);
  }
}

void OpenGLRendererAPI::SetCullFaceEnable(bool enable) {
  if (enable) {
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}

void OpenGLRendererAPI::SetCullFace(CullFaceType cull_face) {
  switch (cull_face) {
    case CullFaceType::Front:
      glCullFace(GL_FRONT);
      break;
    case CullFaceType::Back:
      glCullFace(GL_BACK);
      break;
    case CullFaceType::FrontAndBack:
      glCullFace(GL_FRONT_AND_BACK);
      break;
  }
}

void OpenGLRendererAPI::SetFrontFace(FaceVertexOrder face_vertex_order) {
  switch (face_vertex_order) {
    case FaceVertexOrder::Clockwise:
      glFrontFace(GL_CW);
      break;
    case FaceVertexOrder::CounterClockwise:
      glFrontFace(GL_CCW);
      break;
  }
}

void OpenGLRendererAPI::SetPolygonMode(PolygonMode polygon_mode) {
  switch (polygon_mode) {
    case PolygonMode::Wireframe:
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      break;
    case PolygonMode::Fill:
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
      break;
  }
}

}  // namespace samui
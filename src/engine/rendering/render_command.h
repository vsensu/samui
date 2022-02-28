#ifndef SAMUI_RENDER_COMMAND_H_
#define SAMUI_RENDER_COMMAND_H_

#include "renderer_api.h"

namespace samui {
class SAMUI_API RenderCommand {
 public:
  inline static void Init() { renderer_api_->Init(); }

  inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height) {
    renderer_api_->SetViewport(x, y, width, height);
  }

  inline static void SetClearColor(const glm::vec4& color) {
    renderer_api_->SetClearColor(color);
  }
  inline static void Clear() { renderer_api_->Clear(); }

  inline static void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count = 0) {
    vertex_array->Bind();
    renderer_api_->DrawIndexed(vertex_array,index_count);
  }

  inline static void SetDepthTestEnable(bool enable) { 
    renderer_api_->SetDepthTestEnable(enable);
  }

  inline static void SetCullFaceEnable(bool enable) {
    renderer_api_->SetCullFaceEnable(enable);
  }

  inline static void SetBlendEnable(bool enable) {
    renderer_api_->SetBlendEnable(enable);
  }

  inline static void SetBlendFunc(BlendFactor src_factor, BlendFactor dest_factor) {
    renderer_api_->SetBlendFunc(src_factor, dest_factor);
  }

  inline static void SetCullFace(CullFaceType cull_face) {
    renderer_api_->SetCullFace(cull_face);
  }

  inline static void SetFrontFace(FaceVertexOrder face_vertex_order) {
    renderer_api_->SetFrontFace(face_vertex_order);
  }

  inline static void SetPolygonMode(PolygonMode polygon_mode) {
    renderer_api_->SetPolygonMode(polygon_mode);
  }

 private:
  static RendererAPI* renderer_api_;
};
}  // namespace samui

#endif  // SAMUI_RENDER_COMMAND_H_
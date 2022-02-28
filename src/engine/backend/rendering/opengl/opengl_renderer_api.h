#ifndef SAMUI_OPENGL_RENDERER_API_H_
#define SAMUI_OPENGL_RENDERER_API_H_

// clang-format off
#include <memory>

#include <rendering/renderer_api.h>
// clang-format on

namespace samui {
class SAMUI_API OpenGLRendererAPI : public RendererAPI {
 public:
  void Init() override;
  void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                   uint32_t height) override;
  void SetClearColor(const glm::vec4& color) override;
  void Clear() override;
  void DrawIndexed(const std::shared_ptr<VertexArray>& vertex_array,
                   uint32_t                       index_count = 0) override;
  virtual void SetDepthTestEnable(bool enable) override;
  virtual void SetCullFaceEnable(bool enable) override;
  virtual void SetBlendEnable(bool enable) override;
  virtual void SetBlendFunc(BlendFactor src_factor, BlendFactor dest_factor) override;
  virtual void SetCullFace(CullFaceType cull_face) override;
  virtual void SetFrontFace(FaceVertexOrder face_vertex_order) override;
  virtual void SetPolygonMode(PolygonMode polygon_mode) override;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_RENDERER_API_H_
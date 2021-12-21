#ifndef SAMUI_RENDERER_API_H_
#define SAMUI_RENDERER_API_H_

#include <glm/glm.hpp>

#include "buffer.h"
#include "core/core.h"

namespace samui {
  enum class SAMUI_API CullFaceType {
    Front,
    Back,
    FrontAndBack
  };

  enum class SAMUI_API FaceVertexOrder {
    Clockwise,
    CounterClockwise
  };

  enum class SAMUI_API PolygonMode {
    Wireframe,
    Fill
  };

  enum class SAMUI_API BlendFactor {
    Src_Alpha,
    One_Minus_Src_Alpha,
  };

class SAMUI_API RendererAPI {
 public:
  enum class API { None = 0, OpenGL = 1 };

 public:
  virtual ~RendererAPI() {}

  virtual void Init() = 0;
  virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width,
                           uint32_t height) = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;
  virtual void Clear() = 0;
  virtual void DrawIndexed(const samui::Ref<VertexArray>& vertex_array, uint32_t index_count = 0) = 0;
  virtual void SetDepthTestEnable(bool enable) = 0;
  virtual void SetCullFaceEnable(bool enable) = 0;
  virtual void SetBlendEnable(bool enable) = 0;
  virtual void SetBlendFunc(BlendFactor src_factor, BlendFactor dest_factor) = 0;
  virtual void SetCullFace(CullFaceType cull_face) = 0;
  virtual void SetFrontFace(FaceVertexOrder face_vertex_order) = 0;
  virtual void SetPolygonMode(PolygonMode polygon_mode) = 0;

  inline static API GetAPI() { return api_; }

 private:
  static API api_;
};
}  // namespace samui

#endif  // SAMUI_RENDERER_API_H_
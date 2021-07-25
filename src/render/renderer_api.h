#ifndef SAMUI_RENDERER_API_H_
#define SAMUI_RENDERER_API_H_

#include <glm/glm.hpp>

#include "buffer.h"
#include "core/core.h"

namespace samui {
class SAMUI_API RendererAPI {
 public:
  enum class API { None = 0, OpenGL = 1 };

 public:
  virtual ~RendererAPI() {}

  virtual void Init() = 0;
  virtual void SetClearColor(const glm::vec4& color) = 0;
  virtual void Clear() = 0;
  virtual void DrawIndexed(const samui::Ref<VertexArray>& va) = 0;

  inline static API GetAPI() { return api_; }

 private:
  static API api_;
};
}  // namespace samui

#endif  // SAMUI_RENDERER_API_H_
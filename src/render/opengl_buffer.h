#ifndef SAMUI_OPENGL_BUFFER_H_
#define SAMUI_OPENGL_BUFFER_H_

#ifdef SAMUI_RENDER_OPENGL

#include "buffer.h"
#include "core/core.h"

namespace samui {
class SAMUI_API OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  void Bind() override;
  void UnBind() override;
};
}  // namespace samui

#endif

#endif
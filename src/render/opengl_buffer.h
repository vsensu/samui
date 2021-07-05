#ifndef SAMUI_OPENGL_BUFFER_H_
#define SAMUI_OPENGL_BUFFER_H_

#ifdef SAMUI_RENDER_OPENGL

#include "buffer.h"
#include "core/core.h"

namespace samui {
class SAMUI_API OpenGLVertexBuffer : public VertexBuffer {
 public:
  OpenGLVertexBuffer(float* vertices, uint32_t size);
  virtual ~OpenGLVertexBuffer();
  void Bind() override;
  void UnBind() override;

 private:
  uint32_t buffer_id_{0};
};
class SAMUI_API OpenGLIndexBuffer : public IndexBuffer {

 public:
  OpenGLIndexBuffer(uint32_t* indices, uint32_t size);
  virtual ~OpenGLIndexBuffer();
  uint32_t GetCount() const { return count_; }
  void Bind() override;
  void UnBind() override;

 private:
  uint32_t buffer_id_ {0};
  uint32_t count_ {0};
};
}  // namespace samui

#endif

#endif
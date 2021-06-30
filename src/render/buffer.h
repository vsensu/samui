#ifndef SAMUI_BUFFER_H_
#define SAMUI_BUFFER_H_

#include <cstdint>

#include "core/core.h"

namespace samui {
class SAMUI_API Buffer {
 public:
  virtual ~Buffer() {}

  virtual void Bind() = 0;
  virtual void UnBind() = 0;
};

class SAMUI_API VertexBuffer : public Buffer {
 public:
  virtual ~VertexBuffer() {}

  static VertexBuffer* Create(float* vertices, uint32_t size);
};

class SAMUI_API IndexBuffer : public Buffer {
 public:
  virtual ~IndexBuffer() {}

  static IndexBuffer* Create(int* indices, uint32_t size);
};
}  // namespace samui

#endif
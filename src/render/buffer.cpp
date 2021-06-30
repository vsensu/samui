#include "buffer.h"

#include "opengl_buffer.h"
#include "renderer.h"

namespace samui {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
  switch (Renderer::render_api) {
    case RenderAPI::OpenGL:
      return new OpenGLVertexBuffer(vertices, size);
    default:
      return nullptr;
  }
  return nullptr;
}

IndexBuffer* IndexBuffer::Create(int* indices, uint32_t size) {
  return nullptr;
}

}  // namespace samui

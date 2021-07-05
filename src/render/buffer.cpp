#include "buffer.h"

#include "opengl_buffer.h"
#include "renderer.h"
#include "log/log.h"

namespace samui {
VertexBuffer* VertexBuffer::Create(float* vertices, uint32_t size) {
  switch (Renderer::render_api) {
    case RenderAPI::OpenGL:
      return new OpenGLVertexBuffer(vertices, size);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

IndexBuffer* IndexBuffer::Create(uint32_t* indices, uint32_t count) {
  switch (Renderer::render_api) {
    case RenderAPI::OpenGL:
      return new OpenGLIndexBuffer(indices, count);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

}  // namespace samui

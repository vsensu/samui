#include "frame_buffer.h"

#include "opengl_frame_buffer.h"
#include "renderer.h"

namespace samui {
Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLFrameBuffer>(spec);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}
}  // namespace samui

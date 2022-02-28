// clang-format off
#include "frame_buffer.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_frame_buffer.h>
// clang-format on

namespace samui {
std::shared_ptr<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& spec) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLFrameBuffer>(spec);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}
}  // namespace samui

#include "texture.h"

#include "opengl_texture.h"
#include "renderer.h"

namespace samui {
Ref<Texture2D> Texture2D::Create(const char* path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

}  // namespace samui

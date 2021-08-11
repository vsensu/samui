// clang-format off
#include "texture.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_texture.h>
// clang-format on

namespace samui {
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(width, height);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::filesystem::path &path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

}  // namespace samui

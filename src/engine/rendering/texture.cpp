// clang-format off
#include "texture.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_texture.h>
// clang-format on

namespace samui {
Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureFormat format) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(width, height, format);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

Ref<Texture2D> Texture2D::Create(const std::filesystem::path& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

Ref<Texture2D> Texture2D::Combine(const std::vector<Ref<Texture2D>>& textures,
                                  uint16_t cell_width, uint16_t cell_height,
                                  uint16_t rows, uint16_t columns) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLTexture2D::Combine(textures, cell_width, cell_height, rows,
                                      columns);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

ImageInfo* Texture2D::LoadFile(const std::filesystem::path& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLTexture2D::LoadFile(path);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

}  // namespace samui

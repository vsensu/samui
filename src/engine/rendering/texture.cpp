// clang-format off
#include "texture.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_texture.h>
// clang-format on

namespace samui {
std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height, TextureFormat format) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(width, height, format);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::filesystem::path& path) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLTexture2D>(path);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::Combine(const std::vector<std::shared_ptr<Texture2D>>& textures,
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

ImageInfo* Texture2D::LoadFile(const std::filesystem::path& path, bool flip_vertically) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return OpenGLTexture2D::LoadFile(path, flip_vertically);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

}  // namespace samui

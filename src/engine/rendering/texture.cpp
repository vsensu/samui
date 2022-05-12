// clang-format off
#include "texture.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_texture.h>
// clang-format on

namespace samui
{
std::shared_ptr<Texture2D> Texture2D::create(uint32_t width, uint32_t height,
                                             TextureFormat format)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(width, height, format);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::create(const std::filesystem::path& path)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLTexture2D>(path);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Texture2D> Texture2D::combine(
    const std::vector<std::shared_ptr<Texture2D>>& textures,
    uint16_t cell_width, uint16_t cell_height, uint16_t rows, uint16_t columns)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return OpenGLTexture2D::combine(textures, cell_width, cell_height,
                                            rows, columns);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

ImageInfo* Texture2D::load_file(const std::filesystem::path& path)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return OpenGLTexture2D::load_file(path);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<CubeMap> CubeMap::create(
    const std::array<std::filesystem::path, 6>& paths)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLCubeMap>(paths);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

}  // namespace samui

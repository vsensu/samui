// clang-format off
#include "texture_create.h"

#ifdef SAMUI_RENDER_OPENGL
#include <backend/rendering/opengl/opengl_texture.h>
#endif

#include "renderer.h"
// clang-format on

namespace samui
{
namespace texture2d
{
std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height,
                                  TextureFormat format)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLTexture2D>(width, height, format);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Texture2D> create(const std::filesystem::path& path)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLTexture2D>(path);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Texture2D> combine(
    const std::vector<std::shared_ptr<Texture2D>>& textures,
    uint16_t cell_width, uint16_t cell_height, uint16_t rows, uint16_t columns)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return OpenGLTexture2D::combine(textures, cell_width, cell_height,
                                            rows, columns);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

ImageInfo* load_file(const std::filesystem::path& path)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return OpenGLTexture2D::load_file(path);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}
}  // namespace texture2d

namespace cubemap
{
std::shared_ptr<CubeMap> create(
    const std::array<std::filesystem::path, 6>& paths)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLCubeMap>(paths);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}
}  // namespace cubemap

}  // namespace samui

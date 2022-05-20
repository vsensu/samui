// clang-format off
#include "buffer_create.h"

#ifdef SAMUI_RENDER_OPENGL
#include "../backend/opengl/opengl_buffer.h"
#endif

#include "renderer.h"
// clang-format on

namespace samui
{
namespace vertex_buffer
{

std::shared_ptr<VertexBuffer> create(uint32_t size)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLVertexBuffer>(size);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<VertexBuffer> create(const void* vertices, uint32_t size)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}
}  // namespace vertex_buffer

namespace index_buffer
{

std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t count)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

}  // namespace index_buffer

namespace vertex_array
{
std::shared_ptr<VertexArray> create()
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLVertexArray>();
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}
}  // namespace vertex_array

}  // namespace samui

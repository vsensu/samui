// clang-format off
#include "buffer.h"

#include <log/log.h>
#include <backend/rendering/opengl/opengl_buffer.h>
#include "renderer.h"
// clang-format on

namespace samui
{
std::shared_ptr<VertexBuffer> VertexBuffer::create(uint32_t size)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(size);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* vertices,
                                                   uint32_t    size)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(vertices, size);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<IndexBuffer> IndexBuffer::create(const uint32_t* indices,
                                                 uint32_t        count)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(indices, count);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<VertexArray> VertexArray::create()
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLVertexArray>();
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

}  // namespace samui

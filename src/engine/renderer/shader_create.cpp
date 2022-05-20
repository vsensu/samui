// clang-format off
#include "shader_create.h"

#ifdef SAMUI_RENDER_OPENGL
#include <backend/rendering/opengl/opengl_shader.h>
#endif

#include "renderer.h"
// clang-format on

namespace samui
{
namespace shader
{

std::shared_ptr<Shader> create(const std::string& filepath)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLShader>(filepath);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Shader> create(const std::string& vertex_shader,
                                       const std::string& fragment_shader)
{
    switch (Renderer::get_api())
    {
        case RenderCommand::API::OpenGL:
        {
#ifdef SAMUI_RENDER_OPENGL
            return std::make_shared<OpenGLShader>(vertex_shader,
                                                  fragment_shader);
#endif
        }
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

}  // namespace shader

}  // namespace samui
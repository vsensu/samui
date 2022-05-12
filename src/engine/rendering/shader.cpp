// clang-format off
#include "shader.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_shader.h>
// clang-format on

namespace samui
{

std::shared_ptr<Shader> Shader::create(const std::string& filepath)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(filepath);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

std::shared_ptr<Shader> Shader::create(const std::string& vertex_shader,
                                       const std::string& fragment_shader)
{
    switch (Renderer::get_api())
    {
        case RendererAPI::API::OpenGL:
            return std::make_shared<OpenGLShader>(vertex_shader,
                                                  fragment_shader);
    }
    SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
    return nullptr;
}

bool ShaderLibrary::add(const std::string& name, std::shared_ptr<Shader> shader)
{
    shaders_[name] = shader;
    return true;
}

std::shared_ptr<Shader> ShaderLibrary::get(const std::string& name) const
{
    auto it = shaders_.find(name);
    if (it != shaders_.end())
    {
        return it->second;
    }

    return nullptr;
}

}  // namespace samui

// clang-format off
#include "shader.h"

#include "renderer.h"
#include <backend/rendering/opengl/opengl_shader.h>
// clang-format on

namespace samui {

Ref<Shader> Shader::Create(const std::string& filepath) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLShader>(filepath);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

Ref<Shader> Shader::Create(const std::string& vertex_shader,
                           const std::string& fragment_shader) {
  switch (Renderer::GetAPI()) {
    case RendererAPI::API::OpenGL:
      return std::make_shared<OpenGLShader>(vertex_shader, fragment_shader);
  }
  SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
  return nullptr;
}

bool ShaderLibrary::Add(const std::string& name, Ref<Shader> shader) {
  shaders_[name] = shader;
  return true;
}

Ref<Shader> ShaderLibrary::Get(const std::string& name) const {
  auto it = shaders_.find(name);
  if (it != shaders_.end()) {
    return it->second;
  }

  return nullptr;
}

}  // namespace samui

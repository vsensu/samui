#ifndef SAMUI_OPENGL_SHADER_H_
#define SAMUI_OPENGL_SHADER_H_

// clang-format off
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "gl_errors.h"
#include <rendering/shader.h>
#include <debug/instrumentor.h>
// clang-format on

namespace samui {
class SAMUI_API OpenGLShader : public Shader {
 public:
  OpenGLShader(const std::string& filePath);

  OpenGLShader(const std::string& vertex_shader,
               const std::string& fragment_shader);

  void Bind() override;

  GLint GetUniformLocation(const std::string& name);

  void SetInt(const std::string& name, int value) override;

  void SetIntArray(const std::string& name, int* value,
                   uint32_t count) override;

  void SetUInt(const std::string& name, unsigned int value) override;

  void SetFloat(const std::string& name, float value) override;

  void SetFloat2(const std::string& name, const glm::vec2& value) override;

  void SetFloat3(const std::string& name, const glm::vec3& value) override;

  void SetInt3(const std::string& name, const glm::ivec3& value) override;

  void SetFloat4(const std::string& name, const glm::vec4& value) override;

  void SetMat3(const std::string& name, const glm::mat3& value) override;

  void SetMat4(const std::string& name, const glm::mat4& value) override;

 private:
  std::string                             ReadFile(const std::string& filepath);
  std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
  void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

 private:
  GLuint shaderProgram;
};

}  // namespace samui

#endif  // SAMUI_OPENGL_SHADER_H_

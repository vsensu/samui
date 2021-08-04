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

  void Bind() override { glUseProgram(shaderProgram); }

  inline auto GetUniformLocation(const std::string& name) {
    return glGetUniformLocation(shaderProgram, name.c_str());
  }

  void SetInt(const std::string& name, int value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform1i(GetUniformLocation(name), value));
  }

  void SetIntArray(const std::string& name, int* value,
                   uint32_t count) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform1iv(GetUniformLocation(name), count, value));
  }

  void SetUInt(const std::string& name, unsigned int value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform1ui(GetUniformLocation(name), value));
  }

  void SetFloat(const std::string& name, float value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform1f(GetUniformLocation(name), value));
  }

  void SetFloat2(const std::string& name, const glm::vec2& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform2fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
  }

  void SetFloat3(const std::string& name, const glm::vec3& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform3fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
  }

  void SetInt3(const std::string& name, const glm::ivec3& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform3iv(GetUniformLocation(name), 1, glm::value_ptr(value)));
  }

  void SetFloat4(const std::string& name, const glm::vec4& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniform4fv(GetUniformLocation(name), 1, glm::value_ptr(value)));
  }

  void SetMat3(const std::string& name, const glm::mat3& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE,
                               glm::value_ptr(value)));
  }

  void SetMat4(const std::string& name, const glm::mat4& value) override {
    SAMUI_PROFILE_FUNCTION();
    glCheck(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE,
                               glm::value_ptr(value)));
  }

 private:
  std::string                             ReadFile(const std::string& filepath);
  std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
  void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);

 private:
  GLuint shaderProgram;
};

}  // namespace samui

#endif  // SAMUI_OPENGL_SHADER_H_

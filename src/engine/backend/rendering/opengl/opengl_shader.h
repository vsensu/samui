#pragma once

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

namespace samui
{
class SAMUI_API OpenGLShader : public Shader
{
  public:
    OpenGLShader(const std::string& filePath);

    OpenGLShader(const std::string& vertex_shader,
                 const std::string& fragment_shader);

    void bind() override;

    GLint get_uniform_location(const std::string& name);

    void set_int(const std::string& name, int value) override;

    void set_int_array(const std::string& name, int* value,
                       uint32_t count) override;

    void set_uint(const std::string& name, unsigned int value) override;

    void set_float(const std::string& name, float value) override;

    void set_float2(const std::string& name, const glm::vec2& value) override;

    void set_float3(const std::string& name, const glm::vec3& value) override;

    void set_int3(const std::string& name, const glm::ivec3& value) override;

    void set_float4(const std::string& name, const glm::vec4& value) override;

    void set_mat3(const std::string& name, const glm::mat3& value) override;

    void set_mat4(const std::string& name, const glm::mat4& value) override;

  private:
    std::string read_file(const std::string& filepath);
    std::unordered_map<GLenum, std::string> pre_process(
        const std::string& source);
    void compile(const std::unordered_map<GLenum, std::string>& shaderSources);

  private:
    GLuint shaderProgram;
};

}  // namespace samui

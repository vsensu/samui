#include "opengl_shader.h"

#include "log/log.h"
#include "pch.h"
#include "debug/instrumentor.h"

namespace samui {
GLenum ShaderTypeFromString(const std::string& type) {
  if (type == "vertex") {
    return GL_VERTEX_SHADER;
  }

  if (type == "fragment" || type == "pixel") {
    return GL_FRAGMENT_SHADER;
  }

  SAMUI_ENGINE_ASSERT(false, "Unknown shader type: {0}", type);
  return 0;
}

OpenGLShader::OpenGLShader(const std::string& filePath) {
  SAMUI_PROFILE_FUNCTION();
  std::string source = ReadFile(filePath);
  auto        shader_sources = PreProcess(source);
  Compile(shader_sources);
}

OpenGLShader::OpenGLShader(const std::string& vertex_shader,
                           const std::string& fragment_shader) {
  SAMUI_PROFILE_FUNCTION();
  std::unordered_map<GLenum, std::string> shader_sources;
  shader_sources[GL_VERTEX_SHADER] = vertex_shader;
  shader_sources[GL_FRAGMENT_SHADER] = fragment_shader;
  Compile(shader_sources);
}

std::string OpenGLShader::ReadFile(const std::string& filepath) {
  SAMUI_PROFILE_FUNCTION();
  std::string   result;
  std::ifstream in(filepath, std::ios::in | std::ios::binary);
  if (in) {
    in.seekg(0, std::ios::end);
    result.resize(in.tellg());
    in.seekg(0, std::ios::beg);
    in.read(&result[0], result.size());
    in.close();
  } else {
    SAMUI_ENGINE_ERROR("Could not open file '{0}'", filepath);
  }

  return result;
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(
    const std::string& source) {
  SAMUI_PROFILE_FUNCTION();
  std::unordered_map<GLenum, std::string> shaderSources;

  const char* type_token = "#type";
  size_t      type_token_len = strlen(type_token);
  size_t      pos = source.find(type_token, 0);
  while (pos != std::string::npos) {
    size_t eol = source.find("\r\n", pos);
    SAMUI_ENGINE_ASSERT(eol != std::string::npos, "Syntax Error");
    size_t      begin = pos + type_token_len + 1;
    std::string type = source.substr(begin, eol - begin);
    SAMUI_ENGINE_ASSERT(
        type == "vertex" || type == "fragment" || type == "pixel",
        "Invalid shader type");

    size_t nextLinePos = source.find("\r\n", eol);
    pos = source.find(type_token, nextLinePos);
    shaderSources[ShaderTypeFromString(type)] =
        (pos == std::string::npos)
            ? source.substr(nextLinePos)
            : source.substr(nextLinePos, pos - nextLinePos);
  }

  return shaderSources;
}

void OpenGLShader::Compile(
    const std::unordered_map<GLenum, std::string>& shaderSources) {
      SAMUI_PROFILE_FUNCTION();
  // Create Shader program
  GLuint program = glCreateProgram();
  SAMUI_ENGINE_ASSERT(shaderSources.size() < 3, "Only support 2 shaders now");
  std::array<GLuint, 2> shader_ids;
  int                   shader_id_index = 0;

  for (const auto& pair : shaderSources) {
    auto type = pair.first;
    auto source = pair.second;

    const GLchar* source_cstr = source.c_str();
    GLuint        shader = glCreateShader(type);
    glShaderSource(shader, 1, &source_cstr, nullptr);
    glCompileShader(shader);
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      GLint maxLength = 0;
      glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

      std::vector<GLchar> infoLog(maxLength);
      glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);
      glDeleteShader(shader);
      SAMUI_ENGINE_ERROR("{0}", infoLog.data());
      SAMUI_ENGINE_ASSERT(false, "Shader Compilation failure!");
      break;
    }

    // Attach compiled shader object to the program object.
    // This is very similar to specifying the list of objects for linking in a
    // makefile. Since we don't have a makefile here we emulate this behavior
    // programatically. Only the attached objects take part of the linking
    // process.
    glAttachShader(program, shader);

    shader_ids[shader_id_index++] = shader;
  }

  shaderProgram = program;

  glLinkProgram(shaderProgram);
  GLint is_linked = 0;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &is_linked);
  if (!is_linked) {
    GLint maxLength = 0;
    glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

    std::vector<GLchar> infoLog(maxLength);
    glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &infoLog[0]);

    glDeleteProgram(shaderProgram);

    for (const auto& shader_id : shader_ids) {
      glDeleteShader(shader_id);
    }

    SAMUI_ENGINE_ERROR("{0}", infoLog.data());
    SAMUI_ENGINE_ASSERT(false, "Shader link failure!");
    return;
  }

  // You may ask yourself why do we need to validate a program after it has
  // been successfully linked. The difference is that linking checks for
  // errors based on the combination of shaders while the call above check
  // whether the program can execute given the current pipeline state. In a
  // complex application with multiple shaders and lots of state changes it is
  // better to validate before every draw call. In our simple app we check it
  // just once. Also, you may want to do this check only during development
  // and avoid this overhead in the final product.
  glValidateProgram(shaderProgram);

  // Delete the shaders as they're linked into our program now and no longer
  // necessery
  for (const auto& shader_id : shader_ids) {
    glDeleteShader(shader_id);
  }
}

}  // namespace samui

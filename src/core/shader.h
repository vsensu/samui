#ifndef SAMUI_SHADER_H_
#define SAMUI_SHADER_H_

#include "core/core.h"

#include <glm/glm.hpp>
#include <string>

namespace samui {
class SAMUI_API Shader {
 public:
  virtual ~Shader() {}
  virtual void Bind() = 0;

  static Shader* Create(const std::string& filepath);
  static Shader* Create(const std::string& vertex_shader, const std::string& fragment_shader);

  virtual void UploadUniform(const std::string& name, int value) = 0;
  virtual void UploadUniform(const std::string& name, unsigned int value) = 0;
  virtual void UploadUniform(const std::string& name, float value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::vec2& value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::vec3& value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::ivec3& value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::vec4& value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::mat3& value) = 0;
  virtual void UploadUniform(const std::string& name, const glm::mat4& value) = 0;
};

}  // namespace samui

#endif  // SAMUI_SHADER_H_
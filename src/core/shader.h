#ifndef SAMUI_SHADER_H_
#define SAMUI_SHADER_H_

#include "core/core.h"

#include <glm/glm.hpp>

namespace samui {
class SAMUI_API Shader {
 public:
  virtual ~Shader() {}
  virtual void Bind() = 0;

  static Shader* Create(const char* vertex_shader, const char* fragment_shader);

  virtual void UploadUniform(const std::string& name, const glm::vec3& vector) = 0;
  virtual void UploadUniform(const std::string& name, const glm::mat4& matrix) = 0;
};

}  // namespace samui

#endif  // SAMUI_SHADER_H_
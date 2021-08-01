#ifndef SAMUI_SHADER_H_
#define SAMUI_SHADER_H_

#include <glm/glm.hpp>
#include <string>
#include <unordered_map>

#include "core/core.h"

namespace samui {
class SAMUI_API Shader {
 public:
  virtual ~Shader() {}
  virtual void Bind() = 0;

  static Ref<Shader> Create(const std::string& filepath);
  static Ref<Shader> Create(const std::string& vertex_shader,
                            const std::string& fragment_shader);

  virtual void SetInt(const std::string& name, int value) = 0;
  virtual void SetIntArray(const std::string& name, int* value, uint32_t count) = 0;
  virtual void SetUInt(const std::string& name, unsigned int value) = 0;
  virtual void SetFloat(const std::string& name, float value) = 0;
  virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
  virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
  virtual void SetInt3(const std::string& name, const glm::ivec3& value) = 0;
  virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;
  virtual void SetMat3(const std::string& name, const glm::mat3& value) = 0;
  virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
};

class SAMUI_API ShaderLibrary {
 public:
  bool        Add(const std::string& name, Ref<Shader> shader);
  Ref<Shader> Get(const std::string& name) const;

 private:
  std::unordered_map<std::string, Ref<Shader>> shaders_;
};

}  // namespace samui

#endif  // SAMUI_SHADER_H_
#pragma once

// clang-format off
#include <memory>
#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "rendering_module.h"
// clang-format on

namespace samui
{
class SAMUI_RENDERING_API Shader
{
  public:
    virtual ~Shader() {}
    virtual void bind() = 0;

    // static std::shared_ptr<Shader> create(const std::string& filepath);
    // static std::shared_ptr<Shader> create(const std::string& vertex_shader,
                                          // const std::string& fragment_shader);

    virtual void set_int(const std::string& name, int value) = 0;
    virtual void set_int_array(const std::string& name, int* value,
                               uint32_t count) = 0;
    virtual void set_uint(const std::string& name, unsigned int value) = 0;
    virtual void set_float(const std::string& name, float value) = 0;
    virtual void set_float2(const std::string& name,
                            const glm::vec2&   value) = 0;
    virtual void set_float3(const std::string& name,
                            const glm::vec3&   value) = 0;
    virtual void set_int3(const std::string& name, const glm::ivec3& value) = 0;
    virtual void set_float4(const std::string& name,
                            const glm::vec4&   value) = 0;
    virtual void set_mat3(const std::string& name, const glm::mat3& value) = 0;
    virtual void set_mat4(const std::string& name, const glm::mat4& value) = 0;
};

class SAMUI_RENDERING_API ShaderLibrary
{
  public:
    bool add(const std::string& name, std::shared_ptr<Shader> shader);
    std::shared_ptr<Shader> get(const std::string& name) const;

  private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaders_;
};

}  // namespace samui

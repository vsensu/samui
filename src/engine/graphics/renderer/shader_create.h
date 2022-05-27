#pragma once

// clang-format off
#include <memory>
#include <string>

#include <rendering/shader.h>

#include "renderer_module.h"
// clang-format on

namespace samui
{
namespace shader
{
SAMUI_RENDERER_API std::shared_ptr<Shader> create(const std::string& filepath);
SAMUI_RENDERER_API std::shared_ptr<Shader> create(const std::string& vertex_shader,
                                      const std::string& fragment_shader);
}  // namespace shader
}  // namespace samui

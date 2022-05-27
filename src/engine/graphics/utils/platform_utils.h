#pragma once

// clang-format off
#include <string>

#include "../graphics_module.h"
// clang-format on

namespace samui
{

namespace DialogUtils
{
// These return empty strings if cancelled
std::string SAMUI_GRAPHICS_API open_file(const char* filter);

std::string SAMUI_GRAPHICS_API save_file(const char* filter);
}  // namespace DialogUtils

}  // namespace samui

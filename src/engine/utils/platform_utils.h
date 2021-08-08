#ifndef SAMUI_PLATFORM_UTILS_H_
#define SAMUI_PLATFORM_UTILS_H_

// clang-format off
#include <string>

#include <core/core.h>
// clang-format on

namespace samui {

namespace DialogUtils {
// These return empty strings if cancelled
std::string SAMUI_API OpenFile(const char* filter);

std::string SAMUI_API SaveFile(const char* filter);
}  // namespace DialogUtils

}  // namespace samui

#endif  // SAMUI_PLATFORM_UTILS_H_
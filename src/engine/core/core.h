#pragma once

// clang-format off
#define SAMUI_VERSION 20220401  // year, month, version

#ifdef SAMUI_PLATFORM_WINDOWS
  #ifdef SAMUI_BUILD_DLL
    #define SAMUI_API __declspec(dllexport)
  #else
    #define SAMUI_API __declspec(dllimport)
  #endif
#else
  #error samui only support windows!
#endif

#ifdef SAMUI_DEBUG
  #define SAMUI_ENABLE_ASSERTS
  #define DEBUG_GL_ERRORS
#endif

#ifdef SAMUI_ENABLE_ASSERTS
  #define SAMUI_ASSERT(x, ...) { if(!(x)) {SAMUI_ERROR("Assertion Failed: "##__VA_ARGS__);} }
  #define SAMUI_ENGINE_ASSERT(x, ...) { if(!(x)) {SAMUI_ENGINE_ERROR("Assertion Failed: "##__VA_ARGS__);} }
#else
  #define SAMUI_ASSERT(x, ...)
  #define SAMUI_ENGINE_ASSERT(x, ...)
#endif

// clang-format on

#define BIT(x) (1 << x)

#include <utility>
#define BIND_EVENT_FUNC(fn) [this](auto&& ... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); } 

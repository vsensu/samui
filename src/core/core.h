#ifndef SAMUI_CORE_H_
#define SAMUI_CORE_H_

// clang-format off
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
#endif

#ifdef SAMUI_ENABLE_ASSERTS
  #define SAMUI_ASSERT(x, ...) { if(!(x)) {SAMUI_ERROR("Assertion Failed: {0}", __VA_ARGS__);} }
  #define SAMUI_ENGINE_ASSERT(x, ...) { if(!(x)) {SAMUI_ENGINE_ERROR("Assertion Failed: {0}", __VA_ARGS__);} }
#else
  #define SAMUI_ASSERT(x, ...)
  #define SAMUI_ENGINE_ASSERT(x, ...)
#endif

// clang-format on

#define BIT(x) (1 << x)

#endif
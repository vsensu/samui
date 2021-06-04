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
// clang-format on

#define BIT(x) (1 << x)

#endif
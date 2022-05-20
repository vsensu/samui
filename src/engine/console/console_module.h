#pragma once

// clang-format off
#ifdef SAMUI_PLATFORM_WINDOWS
  #ifdef SAMUI_BUILD_DLL
    #define SAMUI_CONSOLE_API __declspec(dllexport)
  #else
    #define SAMUI_CONSOLE_API __declspec(dllimport)
  #endif
#endif
// clang-format on
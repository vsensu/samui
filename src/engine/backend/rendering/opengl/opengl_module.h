#pragma once

// clang-format off
#ifdef SAMUI_PLATFORM_WINDOWS
  #ifdef SAMUI_BUILD_DLL
    #define SAMUI_OPENGL_API __declspec(dllexport)
  #else
    #define SAMUI_OPENGL_API __declspec(dllimport)
  #endif
#endif

#ifdef SAMUI_DEBUG
  #define DEBUG_GL_ERRORS
#endif
// clang-format on
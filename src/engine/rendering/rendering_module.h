#pragma once

// rendering interface layer 

// clang-format off
#ifdef SAMUI_PLATFORM_WINDOWS
  #ifdef SAMUI_BUILD_DLL
    #define SAMUI_RENDERING_API __declspec(dllexport)
  #else
    #define SAMUI_RENDERING_API __declspec(dllimport)
  #endif
#endif
// clang-format on
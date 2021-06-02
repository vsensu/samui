#ifndef SAMUI_CORE_H_
#define SAMUI_CORE_H_

#ifdef SAMUI_PLATFORM_WINDOWS
  #ifdef SAMUI_BUILD_DLL
    #define SAMUI_API __declspec(dllexport)
  #else
    #define SAMUI_API __declspec(dllimport)
  #endif
#else
  #error samui only support windows!
#endif

#endif
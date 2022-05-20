#pragma once

#ifdef SAMUI_DEBUG
  #define SAMUI_ENABLE_ASSERTS
#endif

#ifdef SAMUI_ENABLE_ASSERTS
  #define SAMUI_ASSERT(x, ...) { if(!(x)) {SAMUI_ERROR("Assertion Failed: "##__VA_ARGS__);} }
  #define SAMUI_ENGINE_ASSERT(x, ...) { if(!(x)) {SAMUI_ENGINE_ERROR("Assertion Failed: "##__VA_ARGS__);} }
#else
  #define SAMUI_ASSERT(x, ...)
  #define SAMUI_ENGINE_ASSERT(x, ...)
#endif
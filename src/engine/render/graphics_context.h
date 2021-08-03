#ifndef SAMUI_GRAPHICS_CONTEXT_H_
#define SAMUI_GRAPHICS_CONTEXT_H_

#include "core/core.h"

namespace samui {
class SAMUI_API GraphicsContext {
 public:
  virtual void Init() = 0;
  virtual void SwapBuffers() = 0;
};
}  // namespace samui

#endif
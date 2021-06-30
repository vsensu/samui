#ifndef SAMUI_RENDERER_H_
#define SAMUI_RENDERER_H_

#include "core/core.h"

namespace samui {
enum class SAMUI_API RenderAPI { None, OpenGL };

class SAMUI_API Renderer {
 public:
  static RenderAPI render_api;
};
}  // namespace samui

#endif
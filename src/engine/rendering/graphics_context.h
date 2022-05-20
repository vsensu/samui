#pragma once

// clang-format off
#include "rendering_module.h"
// clang-format on

namespace samui
{
class SAMUI_RENDERING_API GraphicsContext
{
  public:
    virtual void init() = 0;
    virtual void swap_buffers() = 0;
};
}  // namespace samui

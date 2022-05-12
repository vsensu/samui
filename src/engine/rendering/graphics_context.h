#pragma once

#include "core/core.h"

namespace samui
{
class SAMUI_API GraphicsContext
{
  public:
    virtual void init() = 0;
    virtual void swap_buffers() = 0;
};
}  // namespace samui

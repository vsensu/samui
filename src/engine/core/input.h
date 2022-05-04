#pragma once

#include "core.h"

namespace samui
{
class SAMUI_API Input
{
  public:
    static bool  is_key_pressed(int keycode);
    static bool  is_mouse_button_pressed(int button);
    static float get_mouse_x();
    static float get_mouse_y();
};
}  // namespace samui

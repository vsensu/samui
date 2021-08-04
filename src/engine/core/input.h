#ifndef SAMUI_INPUT_H_
#define SAMUI_INPUT_H_

#include "core.h"

namespace samui {
class SAMUI_API Input {
 public:
  static bool  IsKeyPressed(int keycode);
  static bool  IsMouseButtonPressed(int button);
  static float GetMouseX();
  static float GetMouseY();
};
}  // namespace samui

#endif
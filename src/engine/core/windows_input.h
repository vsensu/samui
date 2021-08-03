#ifndef SAMUI_WINDOWS_INPUT_H_
#define SAMUI_WINDOWS_INPUT_H_

#include "core.h"
#include "input.h"

namespace samui {
class SAMUI_API WindowsInput : public Input {
 protected:
  bool IsKeyPressedImpl(int keycode) override;
  bool IsMouseButtonPressedImpl(int button) override;
  float GetMouseXImpl() override;
  float GetMouseYImpl() override;
};
}  // namespace samui

#endif
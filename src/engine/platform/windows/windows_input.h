#ifndef SAMUI_WINDOWS_INPUT_H_
#define SAMUI_WINDOWS_INPUT_H_

// clang-format off
#include <core/core.h>
#include <core/input.h>
// clang-format on

namespace samui {
class SAMUI_API WindowsInput : public Input {
 protected:
  bool  IsKeyPressedImpl(int keycode) override;
  bool  IsMouseButtonPressedImpl(int button) override;
  float GetMouseXImpl() override;
  float GetMouseYImpl() override;
};
}  // namespace samui

#endif
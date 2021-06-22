#ifndef SAMUI_INPUT_H_
#define SAMUI_INPUT_H_

#include "core.h"

namespace samui {
class SAMUI_API Input {
 public:
  static bool IsKeyPressed(int keycode) {
    return instance_->IsKeyPressedImpl(keycode);
  }

  static bool IsMouseButtonPressed(int button) {
    return instance_->IsMouseButtonPressedImpl(button);
  }

  static float GetMouseX() { return instance_->GetMouseXImpl(); }

  static float GetMouseY() { return instance_->GetMouseYImpl(); }

 protected:
  virtual bool  IsKeyPressedImpl(int keycode) = 0;
  virtual bool  IsMouseButtonPressedImpl(int button) = 0;
  virtual float GetMouseXImpl() = 0;
  virtual float GetMouseYImpl() = 0;

 private:
  static Input* instance_;
};
}  // namespace samui

#endif
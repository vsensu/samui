#pragma once

#include "event.h"

namespace samui
{

class SAMUI_API KeyEvent : public Event
{
  public:
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryKeyboard |
                         EventCategory::EventCategoryInput)

    inline int get_key_code() const { return key_code_; }

  protected:
    KeyEvent(int keycode) : key_code_(keycode) {}
    int key_code_;
};

class SAMUI_API KeyPressedEvent : public KeyEvent
{
  public:
    KeyPressedEvent(const int keycode, int repeatCount)
        : KeyEvent(keycode), repeat_count_(repeatCount)
    {
    }

    inline int  get_repeat_count() const { return repeat_count_; }
    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << key_code_ << " (" << repeat_count_
           << " repeats)";
        return ss.str();
    }
    EVENT_CLASS_TYPE(EventType::KeyPressed)
  private:
    int repeat_count_;
};

class SAMUI_API KeyReleasedEvent : public KeyEvent
{
  public:
    KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << key_code_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::KeyReleased)
};

class SAMUI_API KeyTypedEvent : public KeyEvent
{
  public:
    KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << key_code_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::KeyTyped)
};

}  // namespace samui

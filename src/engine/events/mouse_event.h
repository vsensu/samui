#pragma once

#include "event.h"

namespace samui
{
class SAMUI_API MouseMovedEvent : public Event
{
  public:
    MouseMovedEvent(const float x, const float y) : x_(x), y_(y) {}

    const float get_x() const { return x_; }
    const float get_y() const { return y_; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << x_ << ", " << y_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse |
                         EventCategory::EventCategoryInput)

  private:
    float x_, y_;
};

class SAMUI_API MouseScrolledEvent : public Event
{
  public:
    MouseScrolledEvent(const float xOffset, const float yOffset)
        : x_offset_(xOffset), y_offset_(yOffset)
    {
    }

    const float get_x_offset() const { return x_offset_; }
    const float get_y_offset() const { return y_offset_; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << x_offset_ << ", " << y_offset_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse |
                         EventCategory::EventCategoryInput)

  private:
    float x_offset_, y_offset_;
};

class SAMUI_API MouseButtonEvent : public Event
{
  public:
    int get_mouse_button() const { return button_; }
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryMouse |
                         EventCategory::EventCategoryInput |
                         EventCategory::EventCategoryMouseButton)

  protected:
    MouseButtonEvent(int button) : button_(button) {}
    int button_;
};

class SAMUI_API MouseButtonPressedEvent : public MouseButtonEvent
{
  public:
    MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}
    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << button_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
};

class SAMUI_API MouseButtonReleasedEvent : public MouseButtonEvent
{
  public:
    MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}
    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << button_;
        return ss.str();
    }
    EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
};

}  // namespace samui

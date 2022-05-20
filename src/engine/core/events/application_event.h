#pragma once

// clang-format off
#include "../core_module.h"
#include "../event.h"
// clang-format on

namespace samui
{
class SAMUI_CORE_API WindowResizeEvent : public Event
{
  public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : width_(width), height_(height)
    {
    }

    unsigned int get_width() const { return width_; }
    unsigned int get_height() const { return height_; }

    std::string to_string() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << width_ << ", " << height_;
        return ss.str();
    }

    EVENT_CLASS_TYPE(EventType::WindowResize)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)

  private:
    unsigned int width_;
    unsigned int height_;
};

class SAMUI_CORE_API WindowCloseEvent : public Event
{
  public:
    WindowCloseEvent() = default;
    EVENT_CLASS_TYPE(EventType::WindowClose)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class SAMUI_CORE_API AppTickEvent : public Event
{
  public:
    AppTickEvent() = default;
    EVENT_CLASS_TYPE(EventType::AppTick)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class SAMUI_CORE_API AppUpdateEvent : public Event
{
  public:
    AppUpdateEvent() = default;
    EVENT_CLASS_TYPE(EventType::AppUpdate)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

class SAMUI_CORE_API AppRenderEvent : public Event
{
  public:
    AppRenderEvent() = default;
    EVENT_CLASS_TYPE(EventType::AppRender)
    EVENT_CLASS_CATEGORY(EventCategory::EventCategoryApplication)
};

}  // namespace samui

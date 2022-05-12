#pragma once

// clang-format off
#include <core/core.h>

#include <string>
// clang-format on

namespace samui
{

// clang-format off
enum class EventType {
    None = 0,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    AppTick, AppUpdate, AppRender,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
};

enum EventCategory {
    None = 0,
    EventCategoryApplication =      BIT(0),
    EventCategoryInput =            BIT(1),
    EventCategoryKeyboard =         BIT(2),
    EventCategoryMouse =            BIT(3),
    EventCategoryMouseButton =      BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                  \
  static EventType    get_static_type() { return type; }                          \
  virtual EventType   get_event_type() const override { return get_static_type(); } \
  virtual const char* get_name() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int get_category_flags() const override {return category;}
// clang-format on

class SAMUI_API Event
{
  public:
    virtual EventType   get_event_type() const = 0;
    virtual const char* get_name() const = 0;
    virtual int         get_category_flags() const = 0;
    virtual std::string to_string() const { return get_name(); }

    inline bool is_in_category(EventCategory category) const
    {
        return get_category_flags() & category;
    }

    bool handled_ = false;
};

class EventDispatcher
{
  public:
    EventDispatcher(Event& event) : event_(event) {}

    template <typename T, typename F>
    bool dispatch(const F& func)
    {
        if (event_.get_event_type() == T::get_static_type())
        {
            event_.handled_ |= func(static_cast<T&>(event_));
            return true;
        }
        return false;
    }

  private:
    Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.to_string();
}

}  // namespace samui

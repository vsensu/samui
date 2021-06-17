#ifndef SAMUI_EVENT_H_
#define SAMUI_EVENT_H_

#include "core.h"

namespace samui {

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
    EventCategoryKeyBoard =         BIT(2),
    EventCategoryMouse =            BIT(3),
    EventCategoryMouseButton =      BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                  \
  static EventType    GetStaticType() { return type; }                          \
  virtual EventType   GetEventType() const override { return GetStaticType(); } \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override {return category;}
// clang-format on

class SAMUI_API Event {
 public:
  virtual EventType   GetEventType() const = 0;
  virtual const char* GetName() const = 0;
  virtual int         GetCategoryFlags() const = 0;
  virtual std::string ToString() const { return GetName(); }

  inline bool IsInCategory(EventCategory category) const {
    return GetCategoryFlags() & category;
  }

  bool handled_ = false;
};

class EventDispatcher {
 public:
  EventDispatcher(Event& event) : event_(event) {}

  template <typename T, typename F>
  bool Dispatch(const F& func) {
    if (event_.GetEventType() == T::GetStaticType()) {
      event_.handled_ |= func(static_cast<T&>(event_));
      return true;
    }
    return false;
  }

 private:
  Event& event_;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) {
  return os << e.ToString();
}

}  // namespace samui

#endif
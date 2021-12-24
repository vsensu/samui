#ifndef SAMUI_WINDOW_H_
#define SAMUI_WINDOW_H_

// clang-format off
#include <functional>

#include "core.h"
#include <events/event.h>
// clang-format on

namespace samui {

enum class SAMUI_API InputMode 
{
  GameAndUI,
  GameOnly,
  UIOnly
};

struct WindowProps {
  std::string  Title;
  unsigned int Width;
  unsigned int Height;
  bool Fullscreen;

  WindowProps(const std::string& title = "Samui Engine",
              unsigned int width = 1280, unsigned int height = 720, bool fullscreen = false)
      : Title(title), Width(width), Height(height), Fullscreen(fullscreen) {}
};

class SAMUI_API Window {
 public:
  virtual ~Window() {}

  using EventCallbackFunc = std::function<void(Event&)>;

  virtual void BeforeUpdate() = 0;
  virtual void OnUpdate() = 0;
  virtual void LateUpdate() = 0;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual void SetEventCallback(const EventCallbackFunc& callback) = 0;
  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void* GetNativeWindow() const = 0;

  virtual void SetInputMode(InputMode mode) = 0;

  static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};
}  // namespace samui

#endif
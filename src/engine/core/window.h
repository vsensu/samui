#pragma once

// clang-format off
#include <functional>

#include "core.h"
#include <events/event.h>
// clang-format on

namespace samui
{

enum class SAMUI_API InputMode
{
    GameAndUI,
    GameOnly,
    UIOnly
};

struct WindowProps
{
    std::string  Title;
    unsigned int Width;
    unsigned int Height;
    bool         Fullscreen;

    WindowProps(const std::string& title = "Samui Engine",
                unsigned int width = 1280, unsigned int height = 720,
                bool fullscreen = false)
        : Title(title), Width(width), Height(height), Fullscreen(fullscreen)
    {
    }
};

class SAMUI_API Window
{
  public:
    virtual ~Window() {}

    using EventCallbackFunc = std::function<void(Event&)>;

    virtual void before_update() = 0;
    virtual void on_update() = 0;
    virtual void late_update() = 0;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual void set_event_callback(const EventCallbackFunc& callback) = 0;
    virtual void set_vsync(bool enabled) = 0;
    virtual bool is_vsync() const = 0;

    virtual void* get_native_window() const = 0;

    virtual void set_input_mode(InputMode mode) = 0;

    static std::unique_ptr<Window> create(
        const WindowProps& props = WindowProps());
};
}  // namespace samui

#ifndef SAMUI_WINDOWS_WINDOW_H_
#define SAMUI_WINDOWS_WINDOW_H_

#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <GLFW/glfw3.h>

#include <core/window.h>
#include <rendering/graphics_context.h>
// clang-format on

namespace samui
{
class SAMUI_API WindowsWindow : public Window
{
  public:
    WindowsWindow(const WindowProps& props);
    virtual ~WindowsWindow();

    void before_update() override;
    void on_update() override;
    void late_update() override;

    unsigned int get_width() const override { return data_.Width; }
    unsigned int get_height() const override { return data_.Height; }

    void set_event_callback(const EventCallbackFunc& callback) override
    {
        data_.EventCallback = callback;
    }
    void set_vsync(bool enabled) override;
    bool is_vsync() const override;

    void* get_native_window() const override { return window_; }

    void set_input_mode(InputMode mode) override;

  private:
    virtual void init(const WindowProps& props);
    virtual void shutdown();

  private:
    struct WindowData
    {
        std::string  Title;
        unsigned int Width;
        unsigned int Height;
        bool         VSync;

        EventCallbackFunc EventCallback;
    };

    GLFWwindow*      window_;
    GraphicsContext* graphics_context_;
    // std::unique_ptr<GraphicsContext> context_;
    WindowData data_;
};
}  // namespace samui

#endif

#endif
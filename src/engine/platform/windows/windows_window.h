#ifndef SAMUI_WINDOWS_WINDOW_H_
#define SAMUI_WINDOWS_WINDOW_H_

#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <GLFW/glfw3.h>

#include <core/window.h>
#include <rendering/graphics_context.h>
// clang-format on

namespace samui {
class SAMUI_API WindowsWindow : public Window {
 public:
  WindowsWindow(const WindowProps& props);
  virtual ~WindowsWindow();

  void BeforeUpdate() override;
  void OnUpdate() override;
  void LateUpdate() override;

  unsigned int GetWidth() const override { return data_.Width; }
  unsigned int GetHeight() const override { return data_.Height; }

  void SetEventCallback(const EventCallbackFunc& callback) override {
    data_.EventCallback = callback;
  }
  void SetVSync(bool enabled) override;
  bool IsVSync() const override;

  void* GetNativeWindow() const override { return window_; }

  void SetInputMode(InputMode mode) override;

 private:
  virtual void Init(const WindowProps& props);
  virtual void Shutdown();

 private:
  struct WindowData {
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
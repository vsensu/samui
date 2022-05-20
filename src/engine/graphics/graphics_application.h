#pragma once

// clang-format off
#include <core/application.h>
#include <core/events/application_event.h>

#include "graphics_module.h"
#include "Window.h"
// clang-format on

namespace samui
{

class ImGuiLayer;

class SAMUI_GRAPHICS_API GraphicsApplication : public Application
{
  public:
    explicit GraphicsApplication(const WindowProps& props = WindowProps());
    virtual ~GraphicsApplication();
    virtual void run() override;
    // void push_layer(Layer* layer);
    // void push_overlay(Layer* layer);
    virtual void close() override;

    // inline static Application& instance() { return *instance_; }
    inline Window&             get_window() { return *window_; }
    ImGuiLayer* get_imgui_layer() { return imgui_layer_; }
    void set_input_mode(InputMode mode);

  private:
    virtual void on_event(Event& e) override;
    bool on_window_close(WindowCloseEvent& event);
    bool on_window_resize(WindowResizeEvent& event);

  private:
    std::unique_ptr<Window> window_;
    ImGuiLayer*             imgui_layer_;
    bool                    running_{true};
    bool                    minimized_{false};
    float                   last_frame_time_{0.f};
};

}  // namespace samui

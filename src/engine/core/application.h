#pragma once

// clang-format off
#include "core.h"
#include "Window.h"
#include <events/application_event.h>
#include <layer/layer_stack.h>
// clang-format on

namespace samui
{

class ImGuiLayer;

class SAMUI_API Application
{
  public:
    explicit Application(const WindowProps& props = WindowProps());
    virtual ~Application();
    virtual void run();

    void push_layer(Layer* layer);
    void push_overlay(Layer* layer);

    inline static Application& instance() { return *instance_; }
    inline Window&             get_window() { return *window_; }

    ImGuiLayer* get_imgui_layer() { return imgui_layer_; }

    void close();
    void set_input_mode(InputMode mode);

  private:
    void on_event(Event& e);
    bool on_window_close(WindowCloseEvent& event);
    bool on_window_resize(WindowResizeEvent& event);

  private:
    std::unique_ptr<Window> window_;
    ImGuiLayer*             imgui_layer_;
    bool                    running_{true};
    bool                    minimized_{false};
    LayerStack              layer_stack_;
    float                   last_frame_time_{0.f};
    static Application*     instance_;
};

Application* create_application();

}  // namespace samui

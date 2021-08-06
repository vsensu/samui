#ifndef SAMUI_APPLICATION_H_
#define SAMUI_APPLICATION_H_

// clang-format off
#include "core.h"
#include "Window.h"
#include <events/application_event.h>
#include <layer/layer_stack.h>
// clang-format on

namespace samui {

class ImGuiLayer;

class SAMUI_API Application {
 private:
  /* data */
 public:
  Application(/* args */);
  virtual ~Application();
  virtual void Run();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* layer);

  inline static Application& Get() { return *instance_; }
  inline Window&             GetWindow() { return *window_; }

  ImGuiLayer* GetImGuiLayer() { return imgui_layer_; }

  void Close();

 private:
  void OnEvent(Event& e);
  bool OnWindowClose(WindowCloseEvent& event);
  bool OnWindowResize(WindowResizeEvent& event);

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

#endif
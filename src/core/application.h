#ifndef SAMUI_APPLICATION_H_
#define SAMUI_APPLICATION_H_

#include "../events/application_event.h"
#include "../layer/layer_stack.h"
#include "Window.h"
#include "core.h"

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

 private:
  void OnEvent(Event& e);
  bool OnWindowClose(WindowCloseEvent& event);

 private:
  std::unique_ptr<Window> window_;
  ImGuiLayer*             imgui_layer_;
  bool                    running_{true};
  LayerStack              layer_stack_;
  float                   last_frame_time_{0.f};
  static Application*     instance_;
};

Application* create_application();

}  // namespace samui

#endif
#ifndef SAMUI_APPLICATION_H_
#define SAMUI_APPLICATION_H_

#include "Window.h"
#include "core.h"
#include "events/application_event.h"
#include "layer_stack.h"

namespace samui {

class SAMUI_API Application {
 private:
  /* data */
 public:
  Application(/* args */);
  virtual ~Application();
  virtual void Run();

  void PushLayer(Layer *layer);
  void PushOverlay(Layer *layer);
 private:
  void OnEvent(Event& e);
  bool OnWindowClose(WindowCloseEvent& event);

 private:
  std::unique_ptr<Window> window_;
  bool                    running_{true};
  LayerStack              layer_stack_;
};

Application* create_application();

}  // namespace samui

#endif
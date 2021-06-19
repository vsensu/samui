#include "application.h"

#include "log/log.h"

namespace samui {
#define BIND_EVENT_FUNC(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application(/* args */) {
  instance_ = this;
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
}

Application::~Application() {}

void Application::Run() {
  while (running_) {
    for (Layer* layer : layer_stack_) {
      layer->OnUpdate();
    }
    window_->OnUpdate();
  }
}

void Application::PushLayer(Layer* layer) {
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
  layer_stack_.PushOverlay(layer);
  layer->OnAttach();
}

void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->OnEvent(e);
    if (e.handled_) {
      break;
    }
  }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
  running_ = false;
  return true;
}

}  // namespace samui
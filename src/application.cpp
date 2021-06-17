#include "application.h"

#include "log/log.h"

namespace samui {

#define BIND_EVENT_FUNC(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

Application::Application(/* args */) {
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(OnEvent));
}

Application::~Application() {}

void Application::Run() {
  while (running_) {
    window_->OnUpdate();
  }
}

void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));
  SAMUI_ENGINE_TRACE("{0}", e.ToString());
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
  running_ = false;
  return true;
}

}  // namespace samui
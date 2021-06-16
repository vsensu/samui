#include "application.h"

namespace samui {

Application::Application(/* args */) { window_ = Window::Create(); }

Application::~Application() {}

void Application::Run() {
  while (running_) {
    window_->OnUpdate();
  }
}

}  // namespace samui
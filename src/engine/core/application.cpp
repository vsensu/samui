// clang-format off
#include "application.h"

#include <GLFW/glfw3.h>

#include <log/log.h>
#include "timestep.h"
#include <imgui/imgui_layer.h>
#include <rendering/buffer.h>
#include <rendering/renderer.h>
#include <debug/instrumentor.h>
// clang-format on

namespace samui {
Application* Application::instance_ = nullptr;

Application::Application(/* args */) {
  SAMUI_PROFILE_FUNCTION();

  instance_ = this;
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(Application::OnEvent));

  Renderer::Init();

  imgui_layer_ = new ImGuiLayer();
  PushOverlay(imgui_layer_);
}

Application::~Application() {
  SAMUI_PROFILE_FUNCTION();
  Renderer::Shutdown();
}

void Application::Run() {
  SAMUI_PROFILE_FUNCTION();
  while (running_) {
    SAMUI_PROFILE_SCOPE("RunLoop");
    auto     time = (float)glfwGetTime();
    Timestep delta_time = time - last_frame_time_;
    last_frame_time_ = time;

    window_->BeforeUpdate();

    if (!minimized_) {
      {
        SAMUI_PROFILE_SCOPE("LayerStack OnUpdate");
        for (Layer* layer : layer_stack_) {
          layer->OnUpdate(delta_time);
        }
      }
      {
        SAMUI_PROFILE_SCOPE("LayerStack OnImGuiRender");
        imgui_layer_->Begin();
        for (Layer* layer : layer_stack_) {
          layer->OnImGuiRender();
        }
        imgui_layer_->End();
      }
    }

    window_->OnUpdate();
    window_->LateUpdate();
  }
}

void Application::PushLayer(Layer* layer) {
  SAMUI_PROFILE_FUNCTION();
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
  SAMUI_PROFILE_FUNCTION();
  layer_stack_.PushOverlay(layer);
  layer->OnAttach();
}

void Application::Close()
{
  SAMUI_PROFILE_FUNCTION();
  running_ = false;
}

void Application::OnEvent(Event& e) {
  SAMUI_PROFILE_FUNCTION();
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(
      BIND_EVENT_FUNC(Application::OnWindowClose));
  dispatcher.Dispatch<WindowResizeEvent>(
      BIND_EVENT_FUNC(Application::OnWindowResize));

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

bool Application::OnWindowResize(WindowResizeEvent& event) {
  SAMUI_PROFILE_FUNCTION();
  if (event.GetWidth() == 0 || event.GetHeight() == 0) {
    minimized_ = true;
    return false;
  }

  minimized_ = false;
  Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

  return false;
}

}  // namespace samui
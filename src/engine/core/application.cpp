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

namespace samui
{
Application* Application::instance_ = nullptr;

Application::Application(const WindowProps& props)
{
    SAMUI_PROFILE_FUNCTION();

    instance_ = this;
    window_ = Window::Create(props);
    window_->SetEventCallback(BIND_EVENT_FUNC(Application::on_event));

    Renderer::Init();

    imgui_layer_ = new ImGuiLayer();
    push_overlay(imgui_layer_);
}

Application::~Application()
{
    SAMUI_PROFILE_FUNCTION();
    Renderer::Shutdown();
}

void Application::run()
{
    SAMUI_PROFILE_FUNCTION();
    while (running_)
    {
        SAMUI_PROFILE_SCOPE("RunLoop");
        auto     time = (float)glfwGetTime();
        Timestep delta_time = time - last_frame_time_;
        last_frame_time_ = time;

        window_->BeforeUpdate();

        if (!minimized_)
        {
            {
                SAMUI_PROFILE_SCOPE("LayerStack OnUpdate");
                for (Layer* layer : layer_stack_)
                {
                    layer->OnUpdate(delta_time);
                }
            }
            {
                SAMUI_PROFILE_SCOPE("LayerStack OnImGuiRender");
                imgui_layer_->Begin();
                for (Layer* layer : layer_stack_)
                {
                    layer->OnImGuiRender();
                }
                imgui_layer_->End();
            }
        }

        window_->OnUpdate();
        window_->LateUpdate();
    }
}

void Application::push_layer(Layer* layer)
{
    SAMUI_PROFILE_FUNCTION();
    layer_stack_.push_layer(layer);
    layer->OnAttach();
}

void Application::push_overlay(Layer* layer)
{
    SAMUI_PROFILE_FUNCTION();
    layer_stack_.push_overlay(layer);
    layer->OnAttach();
}

void Application::close()
{
    SAMUI_PROFILE_FUNCTION();
    running_ = false;
}

void Application::set_input_mode(InputMode mode)
{
    window_->SetInputMode(mode);
}

void Application::on_event(Event& e)
{
    SAMUI_PROFILE_FUNCTION();
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(
        BIND_EVENT_FUNC(Application::on_window_close));
    dispatcher.Dispatch<WindowResizeEvent>(
        BIND_EVENT_FUNC(Application::on_window_resize));

    for (auto it = layer_stack_.end(); it != layer_stack_.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.handled_)
        {
            break;
        }
    }
}

bool Application::on_window_close(WindowCloseEvent& event)
{
    running_ = false;
    return true;
}

bool Application::on_window_resize(WindowResizeEvent& event)
{
    SAMUI_PROFILE_FUNCTION();
    if (event.GetWidth() == 0 || event.GetHeight() == 0)
    {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    Renderer::OnWindowResize(event.GetWidth(), event.GetHeight());

    return false;
}

}  // namespace samui
// clang-format off
#include "graphics_application.h"

#include <GLFW/glfw3.h>

#include <log/log.h>
#include <profiler/instrumentor.h>
#include <core/timestep.h>
#include <rendering/buffer.h>

#include "renderer/renderer.h"
#include "imgui_layer/imgui_layer.h"
// clang-format on

namespace samui
{

GraphicsApplication::GraphicsApplication(const WindowProps& props)
{
    SAMUI_PROFILE_FUNCTION();

    window_ = Window::create(props);
    imgui_layer_ = new ImGuiLayer();
}

GraphicsApplication::~GraphicsApplication()
{
    SAMUI_PROFILE_FUNCTION();
    Renderer::shutdown();
}

void GraphicsApplication::init()
{
    SAMUI_PROFILE_FUNCTION();

    window_->set_event_callback(BIND_EVENT_FUNC(GraphicsApplication::on_event));

    Renderer::init();

    push_overlay(imgui_layer_);

    running_ = true;
}

void GraphicsApplication::run()
{
    SAMUI_PROFILE_FUNCTION();
    while (running_)
    {
        SAMUI_PROFILE_SCOPE("RunLoop");
        auto     time = (float)glfwGetTime();
        Timestep delta_time = time - last_frame_time_;
        last_frame_time_ = time;

        window_->before_update();

        if (!minimized_)
        {
            {
                SAMUI_PROFILE_SCOPE("LayerStack OnUpdate");
                for (Layer* layer : layer_stack_)
                {
                    layer->on_update(delta_time);
                }
            }
            {
                SAMUI_PROFILE_SCOPE("LayerStack OnImGuiRender");
                imgui_layer_->begin();
                for (Layer* layer : layer_stack_)
                {
                    layer->on_imgui_render();
                }
                imgui_layer_->end();
            }
        }

        window_->on_update();
        window_->late_update();
    }
}

// void Application::push_layer(Layer* layer)
// {
//     SAMUI_PROFILE_FUNCTION();
//     layer_stack_.push_layer(layer);
//     layer->on_attach();
// }

// void Application::push_overlay(Layer* layer)
// {
//     SAMUI_PROFILE_FUNCTION();
//     layer_stack_.push_overlay(layer);
//     layer->on_attach();
// }

void GraphicsApplication::close()
{
    SAMUI_PROFILE_FUNCTION();
    running_ = false;
}

void GraphicsApplication::set_input_mode(InputMode mode)
{
    window_->set_input_mode(mode);
}

void GraphicsApplication::on_event(Event& e)
{
    SAMUI_PROFILE_FUNCTION();
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowCloseEvent>(
        BIND_EVENT_FUNC(GraphicsApplication::on_window_close));
    dispatcher.dispatch<WindowResizeEvent>(
        BIND_EVENT_FUNC(GraphicsApplication::on_window_resize));

    for (auto it = layer_stack_.end(); it != layer_stack_.begin();)
    {
        (*--it)->on_event(e);
        if (e.handled_)
        {
            break;
        }
    }
}

bool GraphicsApplication::on_window_close(WindowCloseEvent& event)
{
    running_ = false;
    return true;
}

bool GraphicsApplication::on_window_resize(WindowResizeEvent& event)
{
    SAMUI_PROFILE_FUNCTION();
    if (event.get_width() == 0 || event.get_height() == 0)
    {
        minimized_ = true;
        return false;
    }

    minimized_ = false;
    Renderer::on_window_resize(event.get_width(), event.get_height());

    return false;
}

}  // namespace samui
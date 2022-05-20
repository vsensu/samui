// clang-format off
#include "console_application.h"

#include <log/log.h>
#include <profiler/instrumentor.h>
#include <core/timestep.h>
// clang-format on

namespace samui
{
// ConsoleApplication* ConsoleApplication::instance_ = nullptr;

ConsoleApplication::ConsoleApplication()
{
    SAMUI_PROFILE_FUNCTION();

    // instance_ = this;
}

ConsoleApplication::~ConsoleApplication() { SAMUI_PROFILE_FUNCTION(); }

void ConsoleApplication::run()
{
    SAMUI_PROFILE_FUNCTION();
    while (running_)
    {
        SAMUI_PROFILE_SCOPE("RunLoop");
        {
            SAMUI_PROFILE_SCOPE("LayerStack OnUpdate");
            for (Layer* layer : layer_stack_)
            {
                // layer->on_update(delta_time);
            }
        }
    }
}

// void ConsoleApplication::push_layer(Layer* layer)
// {
//     SAMUI_PROFILE_FUNCTION();
//     layer_stack_.push_layer(layer);
//     layer->on_attach();
// }

// void ConsoleApplication::push_overlay(Layer* layer)
// {
//     SAMUI_PROFILE_FUNCTION();
//     layer_stack_.push_overlay(layer);
//     layer->on_attach();
// }

void ConsoleApplication::close()
{
    SAMUI_PROFILE_FUNCTION();
    running_ = false;
}

void ConsoleApplication::on_event(Event& e)
{
    SAMUI_PROFILE_FUNCTION();
    EventDispatcher dispatcher(e);

    for (auto it = layer_stack_.end(); it != layer_stack_.begin();)
    {
        (*--it)->on_event(e);
        if (e.handled_)
        {
            break;
        }
    }
}

}  // namespace samui
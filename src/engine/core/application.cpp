// clang-format off
#include "application.h"

#include <log/log.h>
#include <profiler/instrumentor.h>

#include "timestep.h"
// clang-format on

namespace samui
{
void Application::push_layer(Layer* layer)
{
    SAMUI_PROFILE_FUNCTION();
    layer_stack_.push_layer(layer);
    layer->on_attach();
}

void Application::push_overlay(Layer* layer)
{
    SAMUI_PROFILE_FUNCTION();
    layer_stack_.push_overlay(layer);
    layer->on_attach();
}

}  // namespace samui
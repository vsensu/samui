#pragma once

// clang-format off
#include "core_module.h"
#include "layer.h"
#include "layer_stack.h"
// clang-format on

namespace samui
{

class SAMUI_CORE_API Application
{
  public:
    virtual ~Application() {}
    virtual void run() = 0;

    virtual void push_layer(Layer* layer);
    virtual void push_overlay(Layer* layer);

    virtual void close() = 0;

  protected:
    virtual void on_event(Event& e) = 0;

  protected:
    LayerStack layer_stack_;
};

// Application* create_application();

}  // namespace samui

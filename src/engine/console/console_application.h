#pragma once

// clang-format off
#include <chrono>

#include <core/application.h>

#include "console_module.h"
// clang-format on

namespace samui
{

class SAMUI_CONSOLE_API ConsoleApplication : public Application
{
  public:
    ConsoleApplication();
    virtual ~ConsoleApplication();
    virtual void init() override;
    virtual void run() override;

    // virtual void push_layer(Layer* layer) override;
    // virtual void push_overlay(Layer* layer) override;

    virtual void close() override;

  private:
    virtual void on_event(Event& e) override;

  private:
    bool running_{true};
    std::chrono::time_point<std::chrono::high_resolution_clock>
        last_frame_time_;
};

}  // namespace samui

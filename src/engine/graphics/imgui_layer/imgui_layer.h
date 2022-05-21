#pragma once

// clang-format off
#include <core/core.h>
#include <core/event.h>
#include <core/layer.h>

#include "imgui_layer_module.h"
// clang-format on

namespace samui
{
class SAMUI_IMGUI_API ImGuiLayer : public Layer
{
  public:
    ImGuiLayer();
    virtual ~ImGuiLayer();

    void         on_attach() override;
    void         on_detach() override;
    virtual void on_event(Event& event) override;

    void begin();
    void end();

    void block_events(bool block) { block_events_ = block; }

  private:
    void set_dark_theme_colors();

  private:
    bool block_events_{true};
};

}  // namespace samui

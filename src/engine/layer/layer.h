#pragma once

// clang-format off
#include <core/core.h>
#include <core/timestep.h>
#include <events/event.h>
// clang-format on

namespace samui
{

class SAMUI_API Layer
{
  public:
    Layer(const std::string& debugName = "Layer");
    virtual ~Layer();

    virtual void on_attach() {}
    virtual void on_detach() {}
    virtual void on_update(const Timestep& deltaTime) {}
    virtual void on_imgui_render() {}
    virtual void on_event(Event& event) {}

    inline const std::string& get_name() const { return debug_name_; }

  protected:
    std::string debug_name_;
};

}  // namespace samui

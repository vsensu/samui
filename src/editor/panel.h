#pragma once

// clang-format off
#include <string>

#include <imgui/imgui.h>

#include <engine/core/minimal.h>
#include <engine/core/events/key_event.h>
#include <engine/graphics/ImGuizmo/ImGuizmo.h>
// clang-format on

namespace samui
{
class Panel
{
public:
    Panel() { panel_count_++; }
    virtual ~Panel() { panel_count_--; }
    uint8_t      panel_count() const { return panel_count_; }

    virtual void on_open() {}
    virtual void on_close() {}
    virtual void on_update(const Timestep& deltaTime) {}
    virtual void on_imgui_render() {}
    virtual void on_event(Event& event) {}
    // virtual std::string key() const = 0;
    virtual std::string name() const = 0;

private:
    inline static uint8_t panel_count_ {0};
};
}  // namespace samui

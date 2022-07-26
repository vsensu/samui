#pragma once

// clang-format off
#include "../panel.h"
// clang-format on

namespace samui
{
class SpriteAtlasPanel : public Panel
{
public:
    virtual void on_imgui_render() override;
    // virtual std::string key() const override { return "sprite_atlas_panel"; }
    constexpr static std::string key() { return "sprite_atlas_panel"; }
    virtual std::string name() const override { return "Sprite Atlas Panel"; }
};
}  // namespace samui
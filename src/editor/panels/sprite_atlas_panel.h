#pragma once

// clang-format off
#include "../panel.h"

#include <engine/2d/tile_map/tile_map.h>
// clang-format on

namespace samui
{
class SpriteAtlasPanel : public Panel
{
public:
    virtual void on_open() override;
    virtual void on_update(const Timestep& deltaTime) override;
    virtual void on_imgui_render() override;
    // virtual std::string key() const override { return "sprite_atlas_panel"; }
    constexpr static std::string key() { return "sprite_atlas_panel"; }
    virtual std::string name() const override { return "Sprite Atlas Panel"; }
    std::shared_ptr<TileMap>       tile_map;
    std::shared_ptr<TileMapRender> tile_map_render;
};
}  // namespace samui
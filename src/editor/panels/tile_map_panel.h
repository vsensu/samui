#pragma once

// clang-format off
#include "../panel.h"

#include <engine/2d/tile_map/tile_map.h>
// clang-format on

namespace samui
{
class TileMapPanel : public Panel
{
public:
    virtual void                 on_open() override;
    virtual void                 on_update(const Timestep& deltaTime) override;
    virtual void                 on_imgui_render() override;
    constexpr static std::string key() { return "tile_map_panel"; }
    virtual std::string      name() const override { return "Tile Map Panel"; }
    std::shared_ptr<SpriteAtlas> tile_sprite_atlas;
    std::shared_ptr<TileMap> tile_map;
    std::shared_ptr<TileMapRender> tile_map_render;
};
}  // namespace samui
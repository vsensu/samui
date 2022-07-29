#pragma once

// clang-format off
#include <unordered_map>
#include <memory>
#include <string_view>

#include <basic/types.h>
#include <basic/hash_tuple.h>
#include <rendering/subtexture2d.h>
#include <graphics/renderer/texture_create.h>
#include <graphics/sprite_atlas/sprite_atlas.h>
#include <collision/collision.h>
#include <rendering/frame_buffer.h>

#include "../2d_module.h"
// clang-format on

namespace samui
{

using tile_id_t = u32;
class SAMUI_2D_API TileSet
{
public:
    tile_id_t   register_tile(sprite_id_t sprite_id);
    sprite_id_t get_sprite_id(tile_id_t id) const;
    const std::unordered_map<tile_id_t, sprite_id_t>& tiles_sprite_id() const
    {
        return tiles_sprite_id_;
    }
    void set_sprite_atlas(std::shared_ptr<SpriteAtlas> sprite_atlas)
    {
        sprite_atlas_ = sprite_atlas;
    }
    std::shared_ptr<samui::SubTexture2D> get_tile_sprite(tile_id_t id) const;

private:
    // tile sprite
    // tile collision
    std::shared_ptr<SpriteAtlas>                     sprite_atlas_;
    std::unordered_map<tile_id_t, sprite_id_t>       tiles_sprite_id_;
    std::unordered_map<tile_id_t, CollisionSettings> tiles_collision_;
    tile_id_t                                        next_tile_id_ = 1;
};

using chunk_index_t = std::tuple<i32, i32>;

class SAMUI_2D_API TileChunk
{
public:
    constexpr static i32 kChunkSize{16};

    int tiles[kChunkSize][kChunkSize];

private:
    chunk_index_t chunk_index;
};

class SAMUI_2D_API TileMap
{
public:
    void set_tile_set(std::shared_ptr<TileSet> tile_set)
    {
        tile_set_ = tile_set;
    }
    void set_chunk(chunk_index_t              chunk_index,
                   std::shared_ptr<TileChunk> tile_chunk);
    void set_tile(float x, float y, tile_id_t tile_id);
    void set_tile(int norm_x, int norm_y, tile_id_t tile_id);
    const std::unordered_map<chunk_index_t, std::shared_ptr<TileChunk>,
                             HashTuple>&
    chunks() const
    {
        return chunks_;
    }

    std::shared_ptr<TileSet> tile_set() const { return tile_set_; }
    i32                      tile_size() const { return tile_size_; }
    bool                     is_chunk_loaded(chunk_index_t chunk_index) const
    {
        return chunks_.find(chunk_index) != chunks_.end();
    }

    static std::tuple<chunk_index_t, int, int> world_to_chunk(float x, float y,
                                                              int tile_size);

private:
    std::unordered_map<chunk_index_t, std::shared_ptr<TileChunk>, HashTuple>
                             chunks_;
    std::shared_ptr<TileSet> tile_set_;
    i32                      tile_size_{64};
};

class SAMUI_2D_API TileMapRender
{
public:
    TileMapRender(std::shared_ptr<TileMap> tile_map);
    void render();
    void on_viewport_resize(u32 width, u32 height);

    std::shared_ptr<FrameBuffer> frame_buffer() const { return frame_buffer_; }
    void                         move_origin(float delta_x, float delta_y);
    const glm::vec2&             origin() const { return origin_; }

private:
    glm::vec2                    origin_{0.f, 0.f};
    glm::vec2                    start_origin_{0.f, 0.f};
    std::shared_ptr<TileMap>     tile_map_;
    std::shared_ptr<FrameBuffer> frame_buffer_;
};

}  // namespace samui

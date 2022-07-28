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
    void register_tile(tile_id_t id, sprite_id_t sprite_id);

private:
    // tile sprite
    // tile collision
    std::shared_ptr<SpriteAtlas>                     sprite_atlas_;
    std::unordered_map<tile_id_t, sprite_id_t>       tiles_sprite_id_;
    std::unordered_map<tile_id_t, CollisionSettings> tiles_collision_;
};

using chunk_index_t = std::tuple<i32, i32>;

class SAMUI_2D_API TileChunk
{
public:
    constexpr static u8 kChunkSize{16};

private:
    chunk_index_t chunk_index_;
    int           tiles_[kChunkSize][kChunkSize];
};

class SAMUI_2D_API TileMap
{
public:
    void set_chunk(chunk_index_t              chunk_index,
                   std::shared_ptr<TileChunk> tile_chunk);

private:
    std::unordered_map<chunk_index_t, std::shared_ptr<TileChunk>, HashTuple>
                             chunks_;
    std::shared_ptr<TileSet> tile_set_;
};

class SAMUI_2D_API TileMapRender
{
public:
    TileMapRender();
    TileMapRender(int tile_pixel_size, std::shared_ptr<TileMap> tile_map);
    void on_viewport_resize(u32 width, u32 height);
    void render();

    std::shared_ptr<FrameBuffer> frame_buffer() const { return frame_buffer_; }

private:
    int                          tile_pixel_size_;
    std::shared_ptr<TileMap>     tile_map_;
    std::shared_ptr<FrameBuffer> frame_buffer_;
};

}  // namespace samui

// clang-format off
#include "tile_map.h"

#include <glm/gtc/matrix_transform.hpp>

#include <graphics/renderer/buffer_create.h>
#include <graphics/renderer/renderer2d.h>
#include <graphics/renderer/render_command.h>
// clang-format on

namespace samui
{

void TileSet::register_tile(tile_id_t id, sprite_id_t sprite_id)
{
    tiles_sprite_id_[id] = sprite_id;
    tiles_collision_[id] = CollisionPresets::NoCollision;
}

sprite_id_t TileSet::get_sprite_id(tile_id_t id) const
{
    return tiles_sprite_id_.at(id);
}

void TileMap::set_chunk(chunk_index_t              chunk_index,
                        std::shared_ptr<TileChunk> tile_chunk)
{
    chunks_[chunk_index] = tile_chunk;
}

void TileMap::set_tile(float x, float y, tile_id_t tile_id)
{
    int norm_x = std::floor(x / tile_size_);
    int norm_y = std::floor(y / tile_size_);
    set_tile(norm_x, norm_y, tile_id);
}

void TileMap::set_tile(int norm_x, int norm_y, tile_id_t tile_id)
{
    // calculate chunk index
    i32 chunk_index_x =
        std::floor(static_cast<float>(norm_x) / TileChunk::kChunkSize);
    i32 chunk_index_y =
        std::floor(static_cast<float>(norm_y) / TileChunk::kChunkSize);
    chunk_index_t chunk_index = {chunk_index_x, chunk_index_y};

    if (auto chunk_find = chunks_.find(chunk_index);
        chunk_find != chunks_.end())
    {
        // calculate tile index
        i32 tile_index_x = static_cast<i32>(
            std::floor(norm_x - chunk_index_x * TileChunk::kChunkSize));
        i32 tile_index_y = static_cast<i32>(
            std::floor(norm_y - chunk_index_y * TileChunk::kChunkSize));
        chunk_find->second->tiles[tile_index_x][tile_index_y] = tile_id;
    }
}

std::tuple<chunk_index_t, int, int> TileMap::world_to_chunk(float x, float y, int tile_size)
{
    int norm_x = std::floor(x / tile_size);
    int norm_y = std::floor(y / tile_size);
    i32 chunk_index_x =
        std::floor(static_cast<float>(norm_x) / TileChunk::kChunkSize);
    i32 chunk_index_y =
        std::floor(static_cast<float>(norm_y) / TileChunk::kChunkSize);
    chunk_index_t chunk_index = {chunk_index_x, chunk_index_y};

    // calculate tile index
    i32 tile_index_x = static_cast<i32>(
        std::floor(norm_x - chunk_index_x * TileChunk::kChunkSize));
    i32 tile_index_y = static_cast<i32>(
        std::floor(norm_y - chunk_index_y * TileChunk::kChunkSize));

    return {chunk_index, tile_index_x, tile_index_y};
}

TileMapRender::TileMapRender(std::shared_ptr<TileMap> tile_map)
    : tile_map_(tile_map)
{
    FrameBufferSpecification spec;
    spec.attachments = {
        FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RED_INTEGER,
        FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA,
        FrameBufferTextureFormat::Depth};
    spec.width = 256;
    spec.height = 256;
    frame_buffer_ = samui::frame_buffer::create(spec);
}

void TileMapRender::render()
{
    frame_buffer_->bind();
    RenderCommand::set_clear_color({0.f, 0.3f, 0.f, 0.f});
    RenderCommand::clear();
    // frame_buffer_->clear_attachment(1, static_cast<uint32_t>(entt::null));

    const auto& spec = frame_buffer_->get_specification();
    glm::mat4   projection =
        glm::ortho(0.f, static_cast<float>(spec.width), 0.f,
                   static_cast<float>(spec.height), -1.f, 1.f);

    Renderer2D::begin_scene(projection);

    for (const auto& chunk_pair : tile_map_->chunks())
    {
        auto chunk_index = chunk_pair.first;
        for (i32 i = 0; i < TileChunk::kChunkSize; i++)
        {
            for (i32 j = 0; j < TileChunk::kChunkSize; j++)
            {
                auto tile_id = chunk_pair.second->tiles[i][j];
                if (tile_id != 0)
                {
                    auto tile_set = tile_map_->tile_set();
                    // auto sprite_id = tile_set->get_sprite_id(tile_id);
                    // auto sprite = tile_set->tiles_sprite_[sprite_id];
                    auto pos = glm::vec2{
                        (std::get<0>(chunk_index) * TileChunk::kChunkSize + i) *
                                tile_map_->tile_size() +
                            tile_map_->tile_size() * 0.5f,
                        (std::get<1>(chunk_index) * TileChunk::kChunkSize + j) *
                                tile_map_->tile_size() +
                            tile_map_->tile_size() * 0.5f};
                    if (tile_id == 2)
                    {
                        Renderer2D::draw_quad(
                            origin_ + pos,
                            {tile_map_->tile_size(), tile_map_->tile_size()},
                            {0.0f, 0.0f, 0.8f, 0.5f});
                    }
                    else
                    {
                        Renderer2D::draw_quad(
                            origin_ + pos,
                            {tile_map_->tile_size(), tile_map_->tile_size()},
                            {0.8f, 0.0f, 0.0f, 0.5f});
                    }
                }
            }
        }
    }

    Renderer2D::end_scene();
    frame_buffer_->unbind();
}

void TileMapRender::on_viewport_resize(u32 width, u32 height)
{
    if (const auto& spec = frame_buffer_->get_specification();
        width > 0 && height > 0.0f &&  // zero sized framebuffer is invalid
        (spec.width != width || spec.height != height))
    {
        frame_buffer_->resize(width, height);
    }
}

void TileMapRender::move_origin(float delta_x, float delta_y)
{
    origin_ += glm::vec2{delta_x, -delta_y};
}

}  // namespace samui

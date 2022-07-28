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

void TileMap::set_chunk(chunk_index_t              chunk_index,
                        std::shared_ptr<TileChunk> tile_chunk)
{
    chunks_[chunk_index] = tile_chunk;
}

TileMapRender::TileMapRender() : tile_pixel_size_(32), tile_map_(nullptr)
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

TileMapRender::TileMapRender(int                      tile_pixel_size,
                             std::shared_ptr<TileMap> tile_map)
    : tile_pixel_size_(tile_pixel_size), tile_map_(tile_map)
{
    FrameBufferSpecification spec;
    spec.attachments = {
        FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RED_INTEGER,
        FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA,
        FrameBufferTextureFormat::Depth};
    spec.width = 1280;
    spec.height = 720;
    frame_buffer_ = samui::frame_buffer::create(spec);
}

void TileMapRender::render()
{
    // Resize
    // if (const auto& spec = frame_buffer_->get_specification();
    //     viewport_size_.x > 0.0f &&
    //     viewport_size_.y > 0.0f &&  // zero sized framebuffer is invalid
    //     (spec.width != viewport_size_.x || spec.height != viewport_size_.y))
    // {
    //     frame_buffer_->resize((uint32_t)viewport_size_.x,
    //                           (uint32_t)viewport_size_.y);
    //     editor_camera_.SetViewportSize(viewport_size_.x, viewport_size_.y);
    // }

    frame_buffer_->bind();
    RenderCommand::clear();
    // frame_buffer_->clear_attachment(1, static_cast<uint32_t>(entt::null));

    const auto& spec = frame_buffer_->get_specification();
    glm::mat4   projection =
        glm::ortho(0.f, static_cast<float>(spec.width),
                   -static_cast<float>(spec.height), 0.f, -1.f, 1.f);
    Renderer2D::begin_scene(projection);

    // auto group =
    //     scene_->registry().group<TransformComponent,
    //     SpriteRendererComponent>();
    // for (auto entity : group)
    // {
    //     auto& transform = group.get<TransformComponent>(entity);
    //     auto& sprite = group.get<SpriteRendererComponent>(entity);
    //     Renderer2D::draw_quad(transform.transform(), sprite.texture,
    //                           sprite.tiling_factor, sprite.color,
    //                           static_cast<entity_t>(entity));
    // }
    samui::Renderer2D::draw_quad({32.f, -32.f}, {64.f, 64.f},
                                 {1.0f, 0.0f, 0.0f, 1.0f});

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

}  // namespace samui

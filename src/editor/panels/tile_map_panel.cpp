// clang-format off
#include "tile_map_panel.h"

#include <format>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/log/log.h>
#include <engine/profiler/instrumentor.h>
#include <engine/graphics/renderer/buffer_create.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <engine/core/engine.h>
#include <engine/graphics/graphics_application.h>
#include <engine/graphics/imgui_layer/imgui_layer.h>
#include <engine/graphics/renderer/render_command.h>
// clang-format on

namespace samui
{

void TileMapPanel::on_open()
{
    tile_sprite_atlas =
        std::make_shared<SpriteAtlas>("assets/textures/snake.png");
    tile_sprite_atlas->add_sprite("win", {17, 12}, {128, 128});
    tile_sprite_atlas->add_sprite("lose", {18, 12}, {128, 128});
    auto tile_set = std::make_shared<TileSet>();
    tile_set->register_tile("win");
    tile_set->register_tile("lose");
    tile_set->set_sprite_atlas(tile_sprite_atlas);
    tile_map = std::make_shared<TileMap>();
    tile_map->set_tile_set(tile_set);
    tile_map_render = std::make_shared<TileMapRender>(tile_map);
    tile_map->set_chunk({0, 0}, std::make_shared<TileChunk>());
    tile_map->set_chunk({-1, 0}, std::make_shared<TileChunk>());
    tile_map->set_tile(0.f, 0.f, 1);
    tile_map->set_tile(66.0f, 66.0f, 1);
    tile_map->set_tile(2, 2, 1);
    tile_map->set_tile(-1, 1, 2);
}

void TileMapPanel::on_update(const Timestep& deltaTime)
{
    tile_map_render->render();
}

void TileMapPanel::on_imgui_render()
{
    static ImVec2 scrolling(0.0f, 0.0f);
    static bool   opt_enable_grid = true;
    static bool   opt_enable_context_menu = true;

    auto sprite_sheet = tile_map->tile_set()->sprite_atlas()->sprite_sheet();
    glm::vec2 sprite_sz = {static_cast<float>(sprite_sheet->get_width()),
                           static_cast<float>(sprite_sheet->get_height())};
    float     sprite_scale = 256 / sprite_sz.y;
    ImVec2    canvas_sz_sprite = {sprite_sz.x * sprite_scale, 256};
    // ImGui::BeginChild("sprite", canvas_sz_sprite);
    ImVec2 canvas_p0_sprite = ImGui::GetCursorScreenPos();
    ImGui::InvisibleButton(
        "sprite canvas", canvas_sz_sprite,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool   is_hovered_sprite = ImGui::IsItemHovered();  // Hovered
    ImGuiIO&     io = ImGui::GetIO();
    const ImVec2 mouse_pos_in_canvas_sprite(io.MousePos.x - canvas_p0_sprite.x,
                                            io.MousePos.y - canvas_p0_sprite.y);

    static int sprite_coord_x = 0;
    static int sprite_coord_y = 0;
    static int sprite_size_x = 128;
    static int sprite_size_y = 128;

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && is_hovered_sprite)
    {
        glm::vec2 pos2d = {
            mouse_pos_in_canvas_sprite.x / sprite_scale,
            (canvas_sz_sprite.y - mouse_pos_in_canvas_sprite.y) / sprite_scale};
        sprite_coord_x = pos2d.x / sprite_size_x;
        sprite_coord_y = pos2d.y / sprite_size_y;
        SAMUI_ENGINE_INFO("2d {},{}", pos2d.x, pos2d.y);
    }

    ImDrawList* draw_list_sprite = ImGui::GetWindowDrawList();

    draw_list_sprite->AddImage((ImTextureID)sprite_sheet->get_texture_id(),
                               {canvas_p0_sprite.x + 0, canvas_p0_sprite.y + 0},
                               {canvas_p0_sprite.x + canvas_sz_sprite.x,
                                canvas_p0_sprite.y + canvas_sz_sprite.y},
                               {0.f, 1.f}, {1.f, 0.f});
    glm::vec2 sprite_top_left = {sprite_coord_x * sprite_size_x * sprite_scale,
                                 sprite_coord_y * sprite_size_y * sprite_scale};
    draw_list_sprite->AddRect(
        {canvas_p0_sprite.x + sprite_top_left.x,
         canvas_p0_sprite.y + canvas_sz_sprite.y - (sprite_top_left.y)},
        {canvas_p0_sprite.x + sprite_top_left.x + sprite_size_x * sprite_scale,
         canvas_p0_sprite.y + canvas_sz_sprite.y -
             (sprite_top_left.y + sprite_size_y * sprite_scale)},
        IM_COL32(255, 255, 0, 255));

    // ImGui::EndChild();

    ImGui::SameLine();
    ImGui::BeginGroup();
    ImGui::Columns(2);
    ImGui::DragInt("sprite x", &sprite_coord_x);
    ImGui::NextColumn();
    ImGui::DragInt("sprite y", &sprite_coord_y);
    ImGui::NextColumn();
    ImGui::DragInt("sprite size x", &sprite_size_x);
    ImGui::NextColumn();
    ImGui::DragInt("sprite size y", &sprite_size_y);
    ImGui::Columns(1);
    if (ImGui::Button("Add Tile"))
    {
        auto sprite_id = std::format(
            "{}_new_tile", tile_map->tile_set()->tiles_sprite_id().size() + 1);
        tile_map->tile_set()->sprite_atlas()->add_sprite(
            sprite_id, {sprite_coord_x, sprite_coord_y},
            {sprite_size_x, sprite_size_y});
        tile_map->tile_set()->register_tile(sprite_id);
    }

    static int tile_id = 0;
    // ImGui::RadioButton("remove", &tile_id, 0);
    // for (const auto& tile_pair : tile_map->tile_set()->tiles_sprite_id())
    // {
    //     ImGui::RadioButton(
    //         std::format("{}-{}", tile_pair.first, tile_pair.second).c_str(),
    //         &tile_id, tile_pair.first);
    //     auto tile_sprite =
    //         tile_map->tile_set()->get_tile_sprite(tile_pair.first);
    //     if (tile_sprite != nullptr)
    //     {
    //         ImGui::SameLine();
    //         const auto& tex_coords = tile_sprite->get_tex_coords();
    //         ImGui::Image(
    //             (ImTextureID)tile_sprite->get_texture()->get_texture_id(),
    //             {64, 64}, {tex_coords[0].x, tex_coords[2].y},
    //             {tex_coords[2].x, tex_coords[0].y});
    //     }
    // }

    ImGuiStyle& style = ImGui::GetStyle();
    ImVec2      button_sz(32, 32);
    int         buttons_count = 20;
    float       window_visible_x2 =
        ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
    ImGui::RadioButton("remove", &tile_id, 0);
    auto border_color = style.Colors[ImGuiCol_Border];
    for (const auto& tile_pair : tile_map->tile_set()->tiles_sprite_id())
    {
        ImGui::PushID(tile_pair.first);
        auto tile_sprite =
            tile_map->tile_set()->get_tile_sprite(tile_pair.first);
        const auto& tex_coords = tile_sprite->get_tex_coords();
        auto        texture_id =
            (ImTextureID)tile_sprite->get_texture()->get_texture_id();
        // auto padding = tile_pair.first == tile_id ? 1 : -1;
        style.Colors[ImGuiCol_Border] = tile_pair.first == tile_id
                                            ? ImVec4{1.0f, 0.0f, 0.0f, 1.0f}
                                            : ImVec4{0, 0, 0, 0};
        auto bg_color = tile_pair.first == tile_id ? ImVec4{1, 1, 0, 1}
                                                   : ImVec4{0, 0, 0, 0};
        auto tint_color = tile_pair.first == tile_id ? ImVec4{1, 1, 1, 1}
                                                     : ImVec4{1, 1, 1, 0.4};
        if (ImGui::ImageButton(
                texture_id, button_sz, {tex_coords[0].x, tex_coords[2].y},
                {tex_coords[2].x, tex_coords[0].y}, -1, bg_color, tint_color))
        {
            tile_id = tile_pair.first;
        }
        float last_button_x2 = ImGui::GetItemRectMax().x;
        float next_button_x2 =
            last_button_x2 + style.ItemSpacing.x +
            button_sz.x;  // Expected position if next button was on same line
        if (next_button_x2 < window_visible_x2) ImGui::SameLine();
        ImGui::PopID();
    }
    style.Colors[ImGuiCol_Border] = border_color;
    ImGui::EndGroup();

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::SameLine();
    static int fill_mode = 0;
    ImGui::RadioButton("Fill Mode:Click", &fill_mode, 0);
    ImGui::RadioButton("Fill Mode:Drag", &fill_mode, 1);
    ImGui::RadioButton("Fill Mode:Rect Fill", &fill_mode, 2);
    ImGui::RadioButton("Fill Mode:Rect Frame", &fill_mode, 3);

    // Typically you would use a BeginChild()/EndChild() pair to benefit
    // from a clipping region + own scrolling. Here we demonstrate that this
    // can be replaced by simple offsetting + custom drawing +
    // PushClipRect/PopClipRect() calls. To use a child window instead we
    // could use, e.g:
    //      ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    //      // Disable padding ImGui::PushStyleColor(ImGuiCol_ChildBg,
    //      IM_COL32(50, 50, 50, 255));  // Set a background color
    //      ImGui::BeginChild("canvas", ImVec2(0.0f, 0.0f), true,
    //      ImGuiWindowFlags_NoMove); ImGui::PopStyleColor();
    //      ImGui::PopStyleVar();
    //      [...]
    //      ImGui::EndChild();

    // Using InvisibleButton() as a convenience 1) it will advance the
    // layout cursor and 2) allows us to use IsItemHovered()/IsItemActive()
    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos();     // ImDrawList API uses
                                                        // screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail();  // Resize canvas to
                                                        // what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 =
        ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    tile_map_render->on_viewport_resize(canvas_sz.x, canvas_sz.y);

    // Draw background color
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));

    // draw tilemap
    uint32_t texture_id =
        tile_map_render->frame_buffer()->get_color_attachment_render_id();
    draw_list->AddImage((ImTextureID)texture_id,
                        {canvas_p0.x + 0, canvas_p0.y + 0},
                        {canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y},
                        {0.f, 1.f}, {1.f, 0.f});
    // draw border
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton(
        "canvas", canvas_sz,
        ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const bool   is_hovered = ImGui::IsItemHovered();  // Hovered
    const bool   is_active = ImGui::IsItemActive();    // Held
    const ImVec2 origin(canvas_p0.x + scrolling.x,
                        canvas_p0.y + scrolling.y);  // Lock scrolled origin
    const ImVec2 mouse_pos_in_canvas(io.MousePos.x - origin.x,
                                     io.MousePos.y - origin.y);

    // fill tile
    static glm::vec2 first_fill_pos{};
    static bool      left_mouse_down = false;
    if (fill_mode == 0)
    {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && is_hovered)
        {
            float world_x = io.MousePos.x - canvas_p0.x - scrolling.x;
            float world_y =
                canvas_sz.y - (io.MousePos.y - canvas_p0.y) + scrolling.y;

            auto [chunk_index, tile_index_x, tile_index_y] =
                TileMap::world_to_chunk(world_x, world_y,
                                        tile_map->tile_size());
            if (tile_map->is_chunk_loaded(chunk_index))
            {
                tile_map->set_tile(world_x, world_y, tile_id);
            }
            else
            {
                tile_map->set_chunk(chunk_index, std::make_shared<TileChunk>());
                tile_map->set_tile(world_x, world_y, tile_id);
            }

            SAMUI_ENGINE_INFO("{}, {}", world_x, world_y);
        }
    }
    else if (fill_mode == 1)
    {
        if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Left))
        {
            float world_x = io.MousePos.x - canvas_p0.x - scrolling.x;
            float world_y =
                canvas_sz.y - (io.MousePos.y - canvas_p0.y) + scrolling.y;

            auto [chunk_index, tile_index_x, tile_index_y] =
                TileMap::world_to_chunk(world_x, world_y,
                                        tile_map->tile_size());
            if (tile_map->is_chunk_loaded(chunk_index))
            {
                tile_map->set_tile(world_x, world_y, tile_id);
            }
            else
            {
                tile_map->set_chunk(chunk_index, std::make_shared<TileChunk>());
                tile_map->set_tile(world_x, world_y, tile_id);
            }

            SAMUI_ENGINE_INFO("{}, {}", world_x, world_y);
        }
    }
    else if (fill_mode == 2)
    {
        if (is_hovered)
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                if (!left_mouse_down)
                {
                    left_mouse_down = true;
                    first_fill_pos = {mouse_pos_in_canvas.x,
                                      canvas_sz.y -
                                          (io.MousePos.y - canvas_p0.y) +
                                          scrolling.y};
                }
                else
                {
                }
            }
            else if (left_mouse_down)
            {
                left_mouse_down = false;
                glm::vec2 pos = {
                    mouse_pos_in_canvas.x,
                    canvas_sz.y - (io.MousePos.y - canvas_p0.y) + scrolling.y};
                float min_x = std::min(first_fill_pos.x, pos.x);
                float max_x = std::max(first_fill_pos.x, pos.x);
                float min_y = std::min(first_fill_pos.y, pos.y);
                float max_y = std::max(first_fill_pos.y, pos.y);
                for (float world_x = min_x; world_x <= max_x; ++world_x)
                {
                    for (float world_y = min_y; world_y <= max_y; ++world_y)
                    {
                        auto [chunk_index, tile_index_x, tile_index_y] =
                            TileMap::world_to_chunk(world_x, world_y,
                                                    tile_map->tile_size());
                        if (tile_map->is_chunk_loaded(chunk_index))
                        {
                            tile_map->set_tile(world_x, world_y, tile_id);
                        }
                        else
                        {
                            tile_map->set_chunk(chunk_index,
                                                std::make_shared<TileChunk>());
                            tile_map->set_tile(world_x, world_y, tile_id);
                        }
                    }
                }
            }
        }
    }
    else if (fill_mode == 3)
    {
        if (is_hovered)
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
            {
                if (!left_mouse_down)
                {
                    left_mouse_down = true;
                    first_fill_pos = {mouse_pos_in_canvas.x,
                                      canvas_sz.y -
                                          (io.MousePos.y - canvas_p0.y) +
                                          scrolling.y};
                }
                else
                {
                }
            }
            else if (left_mouse_down)
            {
                left_mouse_down = false;
                glm::vec2 pos = {
                    mouse_pos_in_canvas.x,
                    canvas_sz.y - (io.MousePos.y - canvas_p0.y) + scrolling.y};
                float min_x = std::min(first_fill_pos.x, pos.x);
                float max_x = std::max(first_fill_pos.x, pos.x);
                float min_y = std::min(first_fill_pos.y, pos.y);
                float max_y = std::max(first_fill_pos.y, pos.y);
                for (float world_x = min_x; world_x <= max_x; ++world_x)
                {
                    for (float world_y = min_y; world_y <= max_y; ++world_y)
                    {
                        if (world_x != min_x && world_x != max_x &&
                            world_y != min_y && world_y != max_y)
                            continue;
                            
                        auto [chunk_index, tile_index_x, tile_index_y] =
                            TileMap::world_to_chunk(world_x, world_y,
                                                    tile_map->tile_size());
                        if (tile_map->is_chunk_loaded(chunk_index))
                        {
                            tile_map->set_tile(world_x, world_y, tile_id);
                        }
                        else
                        {
                            tile_map->set_chunk(chunk_index,
                                                std::make_shared<TileChunk>());
                            tile_map->set_tile(world_x, world_y, tile_id);
                        }
                    }
                }
            }
        }
    }

    // Pan (we use a zero mouse threshold when there's no context menu)
    // You may decide to make that threshold dynamic based on whether the
    // mouse is hovering something etc.
    const float mouse_threshold_for_pan =
        opt_enable_context_menu ? -1.0f : 0.0f;
    if (is_active &&
        ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
    {
        scrolling.x += io.MouseDelta.x;
        scrolling.y += io.MouseDelta.y;
        tile_map_render->move_origin(io.MouseDelta.x, io.MouseDelta.y);
    }

    // Context menu (under default mouse threshold)
    ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
    if (opt_enable_context_menu &&
        ImGui::IsMouseReleased(ImGuiMouseButton_Right) &&
        drag_delta.x == 0.0f && drag_delta.y == 0.0f)
        ImGui::OpenPopupOnItemClick("context");
    if (ImGui::BeginPopup("context"))
    {
        if (ImGui::MenuItem("Remove one", NULL, false, true))
        {
        }
        if (ImGui::MenuItem("Remove all", NULL, false, true))
        {
        }
        ImGui::EndPopup();
    }

    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid)
    {
        const float GRID_STEP = 64.0f;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x;
             x += GRID_STEP)
        {
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                               ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        }

        for (float y = canvas_sz.y + fmodf(scrolling.y, GRID_STEP); y > 0;
             y -= GRID_STEP)
        {
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                               ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
        }
    }
    draw_list->PopClipRect();
}
}  // namespace samui

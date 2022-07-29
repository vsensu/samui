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
    static ImVector<ImVec2> points;
    static ImVec2           scrolling(0.0f, 0.0f);
    static bool             opt_enable_grid = true;
    static bool             opt_enable_context_menu = true;
    static bool             adding_line = false;

    if (ImGui::Button("Add Tile"))
    {
        tile_map->tile_set()->register_tile("new_tile");
    }

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    ImGui::Text(
        "Mouse Left: drag to add lines,\nMouse Right: drag to scroll, "
        "click for context menu.");

    static int tile_id = 0;
    ImGui::RadioButton("remove", &tile_id, 0);
    for (const auto& tile_pair : tile_map->tile_set()->tiles_sprite_id())
    {
        ImGui::RadioButton(
            std::format("{}-{}", tile_pair.first, tile_pair.second).c_str(),
            &tile_id, tile_pair.first);
        auto tile_sprite =
            tile_map->tile_set()->get_tile_sprite(tile_pair.first);
        if (tile_sprite != nullptr)
        {
            ImGui::SameLine();
            const auto& tex_coords = tile_sprite->get_tex_coords();
            ImGui::Image(
                (ImTextureID)tile_sprite->get_texture()->get_texture_id(),
                {64, 64}, {tex_coords[0].x, tex_coords[2].y},
                {tex_coords[2].x, tex_coords[0].y});
        }
    }

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
    ImGuiIO&    io = ImGui::GetIO();
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

    // Add first and second point
    if (is_hovered && !adding_line &&
        ImGui::IsMouseClicked(ImGuiMouseButton_Left))
    {
        points.push_back(mouse_pos_in_canvas);
        points.push_back(mouse_pos_in_canvas);
        adding_line = true;
    }
    if (adding_line)
    {
        points.back() = mouse_pos_in_canvas;
        if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) adding_line = false;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && is_hovered)
    {
        float world_x = io.MousePos.x - canvas_p0.x - scrolling.x;
        float world_y =
            canvas_sz.y - (io.MousePos.y - canvas_p0.y) + scrolling.y;

        auto [chunk_index, tile_index_x, tile_index_y] =
            TileMap::world_to_chunk(world_x, world_y, tile_map->tile_size());
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
        if (adding_line) points.resize(points.size() - 2);
        adding_line = false;
        if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0))
        {
            points.resize(points.size() - 2);
        }
        if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0))
        {
            points.clear();
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

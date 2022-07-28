// clang-format off
#include "sprite_atlas_panel.h"

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

void SpriteAtlasPanel::on_update(const Timestep& deltaTime)
{
    RenderCommand::clear();
    Renderer2D::reset_stats();

    glm::mat4 projection = glm::ortho(0.f, 1280.f, -720.f, 0.f, -1.f, 1.f);
    Renderer2D::begin_scene(projection);

    RenderCommand::set_depth_test_enable(true);
    RenderCommand::set_blend_enable(false);
    Renderer2D::draw_quad({32.f, -32.f}, {64.f, 64.f},
                          {1.0f, 0.0f, 0.0f, 1.0f});

    RenderCommand::set_depth_test_enable(false);
    RenderCommand::set_blend_enable(true);
    Renderer2D::draw_quad({48.f, -48.f}, {64.f, 64.f},
                          {0.0f, 1.0f, 0.0f, 0.3f});

    Renderer2D::end_scene();

    tile_map_render.render();
}

void SpriteAtlasPanel::on_imgui_render()
{
    static ImVector<ImVec2> points;
    static ImVec2           scrolling(0.0f, 0.0f);
    static bool             opt_enable_grid = true;
    static bool             opt_enable_context_menu = true;
    static bool             adding_line = false;

    uint32_t texture_id =
        tile_map_render.frame_buffer()->get_color_attachment_render_id();
    // flip uv.y
    ImGui::Image((ImTextureID)texture_id, {256, 256}, {0.f, 1.f}, {1.f, 0.f});

    ImGui::Checkbox("Enable grid", &opt_enable_grid);
    ImGui::Checkbox("Enable context menu", &opt_enable_context_menu);
    ImGui::Text(
        "Mouse Left: drag to add lines,\nMouse Right: drag to scroll, "
        "click for context menu.");

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

    // Draw border and background color
    ImGuiIO&    io = ImGui::GetIO();
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
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
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y),
                               ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y;
             y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y),
                               ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }
    for (int n = 0; n < points.Size; n += 2)
        draw_list->AddLine(
            ImVec2(origin.x + points[n].x, origin.y + points[n].y),
            ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y),
            IM_COL32(255, 255, 0, 255), 2.0f);
    draw_list->PopClipRect();
}
}  // namespace samui

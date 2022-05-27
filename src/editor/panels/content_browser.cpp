// clang-format off
#include "content_browser.h"

#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/renderer/render_command.h>
#include <engine/graphics/renderer/texture_create.h>
#include <imgui/imgui.h>
// clang-format on

namespace samui
{
const std::filesystem::path kAssetPath = "assets";
ContentBrowser::ContentBrowser() : path_(kAssetPath)
{
    folder_icon_ =
        samui::texture2d::create("editor/assets/textures/folder.png");
    file_icon_ = samui::texture2d::create("editor/assets/textures/file.png");
}

void ContentBrowser::OnImGuiRender()
{
    ImGui::Begin("Content Browser");
    if (path_ != kAssetPath)
    {
        if (ImGui::Button(".."))
        {
            path_ = path_.parent_path();
        }
    }

    static float padding = 16.f;
    static float thumbnail_size = 128.f;
    float        cell_size = thumbnail_size + padding;

    float panel_width = ImGui::GetContentRegionAvail().x;
    int   column_count = (int)ceil(panel_width / cell_size);
    if (column_count < 1)
    {
        column_count = 1;
    }

    ImGui::Columns(column_count, 0, false);

    // todo: path_ not exists cause crash
    for (const auto& dir_entry : std::filesystem::directory_iterator(path_))
    {
        const auto& path = dir_entry.path().filename();

        ImGui::PushID(path.string().c_str());
        std::shared_ptr<Texture2D> icon =
            dir_entry.is_directory() ? folder_icon_ : file_icon_;
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::ImageButton((ImTextureID)icon->get_texture_id(),
                           ImVec2(thumbnail_size, thumbnail_size), {0, 1},
                           {1, 0});
        if (ImGui::BeginDragDropSource())
        {
            auto abs_path = std::filesystem::absolute(dir_entry.path());
            const wchar_t* path = abs_path.c_str();
            ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", path,
                                      (wcslen(path) + 1) * sizeof(wchar_t));
            ImGui::EndDragDropSource();
        }
        ImGui::PopStyleColor();
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0))
        {
            if (dir_entry.is_directory())
            {
                path_ /= path;
            }
        }
        ImGui::TextWrapped(path.string().c_str());
        ImGui::NextColumn();
        ImGui::PopID();
    }

    ImGui::Columns(1);
    ImGui::End();
}

}  // namespace samui

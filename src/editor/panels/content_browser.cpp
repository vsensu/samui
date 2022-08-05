// clang-format off
#include "content_browser.h"

#include <algorithm>
#include <execution>

#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/renderer/render_command.h>
#include <engine/graphics/renderer/texture_create.h>
#include <imgui/imgui.h>
// clang-format on

namespace samui
{
std::size_t replace_all(std::string& inout, std::string_view what, std::string_view with)
{
    std::size_t count{};
    for (std::string::size_type pos{};
         inout.npos != (pos = inout.find(what.data(), pos, what.length()));
         pos += with.length(), ++count) {
        inout.replace(pos, what.length(), with.data(), with.length());
    }
    return count;
}
 
std::size_t remove_all(std::string& inout, std::string_view what) {
    return replace_all(inout, what, "");
}

const std::filesystem::path kAssetPath = "assets";
ContentBrowser::ContentBrowser() : path_(root_)
{
    folder_icon_ =
        samui::texture2d::create("editor/assets/textures/folder.png");
    file_icon_ = samui::texture2d::create("editor/assets/textures/file.png");
}

void ContentBrowser::on_imgui_render()
{
    static std::string hovered_path = "";
    if (path_ != root_)
    {
        if (ImGui::Button(".."))
        {
            path_ = path_.parent_path();
        }
    }
    hovered_path = "";
    ImGui::BeginChild("assets view",
                      ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

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

    if (!path_.empty() && std::filesystem::exists(path_))
    {
        for (const auto& dir_entry : std::filesystem::directory_iterator(path_))
        {
            const auto& path = dir_entry.path().filename();

            ImGui::PushID(path.string().c_str());
            std::shared_ptr<Texture2D> icon =
                dir_entry.is_directory() ? folder_icon_ : file_icon_;
            ImGui::PushStyleColor(ImGuiCol_Button,
                                  ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
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
            if (ImGui::IsItemHovered())
            {
                auto abs_path = dir_entry.path().string();
                remove_all(abs_path, std::filesystem::absolute(root_).string());
                hovered_path = abs_path;
                if (ImGui::IsMouseDoubleClicked(0))
                {
                    if (dir_entry.is_directory())
                    {
                        path_ /= path;
                    }
                }
            }
            ImGui::TextWrapped(path.string().c_str());
            ImGui::NextColumn();
            ImGui::PopID();
        }
    }

    ImGui::Columns(1);
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::SameLine();
    ImGui::Text(hovered_path.c_str());
}

void ContentBrowser::set_root(const std::filesystem::path& path)
{
    root_ = path;
    path_ = root_;
}

}  // namespace samui

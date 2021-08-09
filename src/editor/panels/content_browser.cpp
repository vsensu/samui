// clang-format off
#include "content_browser.h"

#include <samui.h>
// clang-format on

namespace samui {
const std::filesystem::path kAssetPath = "assets";
ContentBrowser::ContentBrowser() : path_(kAssetPath) {
  folder_icon_ = Texture2D::Create("editor/assets/textures/folder.png");
  file_icon_ = Texture2D::Create("editor/assets/textures/file.png");
}

void ContentBrowser::OnImGuiRender() {
  ImGui::Begin("Content Browser");
  if (path_ != kAssetPath) {
    if (ImGui::Button("..")) {
      path_ = path_.parent_path();
    }
  }

  static float padding = 16.f;
  static float thumbnail_size = 128.f;
  float        cell_size = thumbnail_size + padding;

  float panel_width = ImGui::GetContentRegionAvail().x;
  int   column_count = (int)ceil(panel_width / cell_size);
  if (column_count < 1) {
    column_count = 1;
  }

  ImGui::Columns(column_count, 0, false);

  for (const auto& dir_entry : std::filesystem::directory_iterator(path_)) {
    const auto&    path = dir_entry.path().filename();
    Ref<Texture2D> icon = dir_entry.is_directory() ? folder_icon_ : file_icon_;
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
    ImGui::ImageButton((ImTextureID)icon->GetTextureID(),
                       ImVec2(thumbnail_size, thumbnail_size), {0, 1}, {1, 0});
    ImGui::PopStyleColor();
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
      if (dir_entry.is_directory()) {
        path_ /= path;
      }
    }
    ImGui::TextWrapped(path.string().c_str());
    ImGui::NextColumn();
  }

  ImGui::Columns(1);
  ImGui::End();
}

}  // namespace samui

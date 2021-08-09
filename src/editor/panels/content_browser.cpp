// clang-format off
#include "content_browser.h"

#include <samui.h>
// clang-format on

namespace samui {
const std::filesystem::path kAssetPath = "assets";
ContentBrowser::ContentBrowser() : path_(kAssetPath) {}

void ContentBrowser::OnImGuiRender() {
  ImGui::Begin("Content Browser");
  if (path_ != kAssetPath) {
    if (ImGui::Button("..")) {
      path_ = path_.parent_path();
    }
  }

  for (const auto& dir_entry : std::filesystem::directory_iterator(path_)) {
    const auto& path = dir_entry.path().filename();
    if (dir_entry.is_directory()) {
      if (ImGui::Button(path.string().c_str())) {
        path_ /= path;
      }
    } else {
      ImGui::Text("%s", path.string().c_str());
    }
  }
  ImGui::End();
}

}  // namespace samui

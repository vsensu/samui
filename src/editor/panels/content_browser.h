#ifndef SAMUI_CONTENT_BROWSER_H_
#define SAMUI_CONTENT_BROWSER_H_

// clang-format off
#include <samui.h>

#include <filesystem>
// clang-format on

namespace samui {
class ContentBrowser {
 public:
  ContentBrowser();

  void OnImGuiRender();

 private:
  std::filesystem::path path_;

  Ref<Texture2D> folder_icon_;
  Ref<Texture2D> file_icon_;
};
}  // namespace samui

#endif  // SAMUI_CONTENT_BROWSER_H_
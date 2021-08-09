#ifndef SAMUI_CONTENT_BROWSER_H_
#define SAMUI_CONTENT_BROWSER_H_

// clang-format off
#include <filesystem>
// clang-format on

namespace samui {
class ContentBrowser {
 public:
  ContentBrowser();

  void OnImGuiRender();

 private:
  std::filesystem::path path_;
};
}  // namespace samui

#endif  // SAMUI_CONTENT_BROWSER_H_
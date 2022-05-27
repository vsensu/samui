#pragma once

// clang-format off
#include <memory>

// #include <samui.h>

#include <filesystem>

#include <engine/rendering/texture.h>
// clang-format on

namespace samui
{
class ContentBrowser
{
  public:
    ContentBrowser();

    void OnImGuiRender();

  private:
    std::filesystem::path path_;

    std::shared_ptr<Texture2D> folder_icon_;
    std::shared_ptr<Texture2D> file_icon_;
};
}  // namespace samui

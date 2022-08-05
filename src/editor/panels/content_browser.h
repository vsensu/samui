#pragma once

// clang-format off
#include <memory>

#include <filesystem>

#include <engine/rendering/texture.h>

#include "../panel.h"
// clang-format on

namespace samui
{
class ContentBrowser : public Panel
{
public:
    ContentBrowser();

    virtual void                 on_imgui_render() override;
    constexpr static std::string key() { return "content_browser_panel"; }
    virtual std::string          name() const override
    {
        return "Content Browser Panel";
    }

    void set_root(const std::filesystem::path& path);

private:
    std::filesystem::path root_;
    std::filesystem::path path_;

    std::shared_ptr<Texture2D> folder_icon_;
    std::shared_ptr<Texture2D> file_icon_;
};
}  // namespace samui

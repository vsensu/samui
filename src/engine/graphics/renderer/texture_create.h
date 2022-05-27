#pragma once

// clang-format off
#include <memory>
#include <filesystem>
#include <array>

#include <rendering/texture.h>

#include "renderer_module.h"
// clang-format on

namespace samui
{
namespace texture2d
{
SAMUI_RENDERER_API std::shared_ptr<Texture2D> create(uint32_t width, uint32_t height,
                                  TextureFormat format = TextureFormat::RGBA);
SAMUI_RENDERER_API std::shared_ptr<Texture2D> create(const std::filesystem::path& path);

SAMUI_RENDERER_API std::shared_ptr<Texture2D> combine(
    const std::vector<std::shared_ptr<Texture2D>>& textures,
    uint16_t cell_width, uint16_t cell_height, uint16_t rows, uint16_t columns);

SAMUI_RENDERER_API ImageInfo* load_file(const std::filesystem::path& path);
}  // namespace texture2d

namespace cubemap
{
SAMUI_RENDERER_API std::shared_ptr<CubeMap> create(
    const std::array<std::filesystem::path, 6>& paths);
}

}  // namespace samui

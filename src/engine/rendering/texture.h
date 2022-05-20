#pragma once

// clang-format off
#include <memory>
#include <filesystem>
#include <array>

#include "rendering_module.h"
// clang-format on

namespace samui
{
class SAMUI_RENDERING_API Texture
{
  public:
    virtual ~Texture() = default;

    virtual uint32_t get_width() const = 0;
    virtual uint32_t get_height() const = 0;

    virtual void set_data(void* data, uint32_t size) = 0;

    virtual void bind(uint8_t slot = 0) = 0;

    virtual bool operator==(const Texture& other) const = 0;

    virtual uint32_t get_texture_id() const = 0;
};

struct SAMUI_RENDERING_API ImageInfo
{
    void* data;
    int   width, height, channels;
};

enum class SAMUI_RENDERING_API TextureFormat
{
    RGB,
    RGBA,
};

class SAMUI_RENDERING_API Texture2D : public Texture
{
  public:
    // static std::shared_ptr<Texture2D> create(
    //     uint32_t width, uint32_t height,
    //     TextureFormat format = TextureFormat::RGBA);
    // static std::shared_ptr<Texture2D> create(const std::filesystem::path& path);

    // static std::shared_ptr<Texture2D> combine(
    //     const std::vector<std::shared_ptr<Texture2D>>& textures,
    //     uint16_t cell_width, uint16_t cell_height, uint16_t rows,
    //     uint16_t columns);

    // static ImageInfo* load_file(const std::filesystem::path& path);
};

class SAMUI_RENDERING_API CubeMap : public Texture
{
  public:
    // static std::shared_ptr<CubeMap> create(
        // const std::array<std::filesystem::path, 6>& paths);
};

}  // namespace samui

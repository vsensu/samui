#pragma once

// clang-format off
#include <memory>

#include <glad/glad.h>

#include <rendering/texture.h>
// clang-format on

namespace samui
{
class SAMUI_API OpenGLTexture2D : public Texture2D
{
  public:
    OpenGLTexture2D(uint32_t width, uint32_t height,
                    TextureFormat format = TextureFormat::RGBA);
    OpenGLTexture2D(const std::filesystem::path& path);
    virtual ~OpenGLTexture2D();

    virtual uint32_t get_width() const override { return width_; }
    virtual uint32_t get_height() const override { return height_; }
    virtual void     set_data(void* data, uint32_t size) override;
    virtual void     bind(uint8_t slot = 0) override;
    virtual bool     operator==(const Texture& other) const override
    {
        return texture_id_ ==
               static_cast<const OpenGLTexture2D&>(other).texture_id_;
    }

    virtual uint32_t get_texture_id() const override { return texture_id_; }

    static std::shared_ptr<Texture2D> combine(
        const std::vector<std::shared_ptr<Texture2D>>& textures,
        uint16_t cell_width, uint16_t cell_height, uint16_t rows,
        uint16_t columns);
    static ImageInfo* load_file(const std::filesystem::path& path);

  private:
    std::filesystem::path path_;
    uint32_t              width_{0};
    uint32_t              height_{0};
    unsigned              texture_id_;
    GLenum                internal_format_;
    TextureFormat         format_;
};

class OpenGLCubeMap : public CubeMap
{
  public:
    OpenGLCubeMap(const std::array<std::filesystem::path, 6>& paths);
    virtual uint32_t get_width() const override { return 64; }
    virtual uint32_t get_height() const override { return 64; }
    virtual void     set_data(void* data, uint32_t size) override {}
    virtual void     bind(uint8_t slot = 0) override;

    virtual bool operator==(const Texture& other) const override
    {
        return texture_id_ ==
               static_cast<const OpenGLCubeMap&>(other).texture_id_;
    }

    virtual uint32_t get_texture_id() const override { return texture_id_; }

  private:
    unsigned texture_id_;
};

}  // namespace samui

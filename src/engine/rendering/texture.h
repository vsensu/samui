#ifndef SAMUI_TEXTURE_H_
#define SAMUI_TEXTURE_H_

// clang-format off
#include <memory>
#include <filesystem>
#include <array>

#include "core/core.h"
// clang-format on

namespace samui {
class SAMUI_API Texture {
 public:
  virtual ~Texture() = default;

  virtual uint32_t GetWidth() const = 0;
  virtual uint32_t GetHeight() const = 0;

  virtual void SetData(void* data, uint32_t size) = 0;

  virtual void Bind(uint8_t slot = 0) = 0;

  virtual bool operator==(const Texture& other) const = 0;

  virtual uint32_t GetTextureID() const = 0;
};

struct SAMUI_API ImageInfo {
  void* data;
  int   width, height, channels;
};

enum class SAMUI_API TextureFormat {
  RGB, RGBA,
};

class SAMUI_API Texture2D : public Texture {
 public:
  static std::shared_ptr<Texture2D> Create(uint32_t width, uint32_t height, TextureFormat format = TextureFormat::RGBA);
  static std::shared_ptr<Texture2D> Create(const std::filesystem::path& path);

  static std::shared_ptr<Texture2D> Combine(const std::vector<std::shared_ptr<Texture2D>>& textures,
                                uint16_t cell_width, uint16_t cell_height,
                                uint16_t rows, uint16_t columns);

  static ImageInfo* LoadFile(const std::filesystem::path& path);
};

class SAMUI_API CubeMap : public Texture {
  public:
  static std::shared_ptr<CubeMap> Create(const std::array<std::filesystem::path, 6> &paths);
};

}  // namespace samui

#endif  // SAMUI_TEXTURE_H_
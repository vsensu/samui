#ifndef SAMUI_OPENGL_TEXTURE_H_
#define SAMUI_OPENGL_TEXTURE_H_

// clang-format off
#include <glad/glad.h>

#include <rendering/texture.h>
// clang-format on

namespace samui {
class SAMUI_API OpenGLTexture2D : public Texture2D {
 public:
  OpenGLTexture2D(uint32_t width, uint32_t height);
  OpenGLTexture2D(const std::string& path);
  virtual ~OpenGLTexture2D();

  virtual uint32_t GetWidth() const override { return width_; }
  virtual uint32_t GetHeight() const override { return height_; }
  virtual void     SetData(void* data, uint32_t size) override;
  virtual void     Bind(uint8_t slot = 0) override;
  virtual bool     operator==(const Texture& other) const override {
    return texture_id_ ==
           static_cast<const OpenGLTexture2D&>(other).texture_id_;
  }

  static unsigned int GetOpenGLTextureEnum(uint8_t slot);

 private:
  std::string path_;
  uint32_t    width_{0};
  uint32_t    height_{0};
  unsigned    texture_id_;
  GLenum      internal_format_;
};

}  // namespace samui

#endif  // SAMUI_OPENGL_TEXTURE_H_
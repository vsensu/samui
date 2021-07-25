#ifndef SAMUI_OPENGL_TEXTURE_H_
#define SAMUI_OPENGL_TEXTURE_H_

#include "texture.h"

namespace samui {
class SAMUI_API OpenGLTexture2D : public Texture2D {
 public:
  OpenGLTexture2D(const std::string& path);
  virtual ~OpenGLTexture2D();

  virtual uint32_t GetWidth() const override { return width_; }
  virtual uint32_t GetHeight() const override { return height_; }
  virtual void     Bind(uint8_t slot = 0) override;

  static unsigned int GetOpenGLTextureEnum(uint8_t slot);

 private:
  std::string path_;
  uint32_t    width_{0};
  uint32_t    height_{0};
  unsigned    texture_id_;
};

}  // namespace samui

#endif  // SAMUI_OPENGL_TEXTURE_H_
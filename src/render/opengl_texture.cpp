#include "opengl_texture.h"

#include <stb_image/stb_image.h>

#include "../debug/instrumentor.h"
#include "../log/log.h"

namespace samui {
OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height)
    : width_(width), height_(height), internal_format_(GL_RGBA) {
  SAMUI_PROFILE_FUNCTION();
  // 创建纹理对象
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
    : path_(path), internal_format_(0) {
  SAMUI_PROFILE_FUNCTION();
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = nullptr;
  {
    SAMUI_PROFILE_SCOPE("stbi_load OpenGLTexture2D::OpenGLTexture2D(const std::string& path)");
    data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  }
  SAMUI_ENGINE_ASSERT(data, "Failed to load image:{0}", path);
  width_ = width;
  height_ = height;

  if (channels == 4) {
    internal_format_ = GL_RGBA;
  } else if (channels == 3) {
    internal_format_ = GL_RGB;
  }
  SAMUI_ENGINE_ASSERT(internal_format_, "Format not supported!");

  // 创建纹理对象
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0 /*mipmap*/, internal_format_, width, height,
               0 /*legacy*/, internal_format_, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() {
  SAMUI_PROFILE_FUNCTION();
  glDeleteTextures(1, &texture_id_);
}

unsigned int OpenGLTexture2D::GetOpenGLTextureEnum(uint8_t slot) {
  return GL_TEXTURE0 + slot;
}

void OpenGLTexture2D::SetData(void* data, uint32_t size) {
  SAMUI_PROFILE_FUNCTION();
  uint32_t bpp = internal_format_ == GL_RGBA ? 4 : 3;
  SAMUI_ENGINE_ASSERT(size == width_ * height_ * bpp,
                      "data must be entire texture!");
  glTexImage2D(GL_TEXTURE_2D, 0 /*mipmap*/, internal_format_, width_, height_,
               0 /*legacy*/, internal_format_, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void OpenGLTexture2D::Bind(uint8_t slot) {
  SAMUI_PROFILE_FUNCTION();
  // 绑定纹理
  glActiveTexture(GetOpenGLTextureEnum(slot));
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

}  // namespace samui

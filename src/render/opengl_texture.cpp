#include "opengl_texture.h"

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include "../log/log.h"

namespace samui {
OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : path_(path) {
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  auto* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
  SAMUI_ENGINE_ASSERT(data, "Failed to load image:{0}", path);
  width_ = width;
  height_ = height;

  // 创建纹理对象
  glGenTextures(1, &texture_id_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0 /*mipmap*/, GL_RGB, width, height, 0 /*legacy*/,
               GL_RGB, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
  stbi_image_free(data);
}

OpenGLTexture2D::~OpenGLTexture2D() { glDeleteTextures(1, &texture_id_); }

unsigned int OpenGLTexture2D::GetOpenGLTextureEnum(uint8_t slot) {
  return GL_TEXTURE0 + slot;
}

void OpenGLTexture2D::Bind(uint8_t slot) {
  // 绑定纹理
  glActiveTexture(GetOpenGLTextureEnum(slot));
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

}  // namespace samui

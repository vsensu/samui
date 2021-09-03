// clang-format off
#include "opengl_texture.h"

#include <stb_image.h>

#include <log/log.h>
#include <debug/instrumentor.h>
// clang-format on

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

OpenGLTexture2D::OpenGLTexture2D(const std::filesystem::path& path)
    : path_(path), internal_format_(0) {
  SAMUI_PROFILE_FUNCTION();
  int width, height, channels;
  stbi_set_flip_vertically_on_load(1);
  stbi_uc* data = nullptr;
  {
    SAMUI_PROFILE_SCOPE("stbi_load");
    data = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
  }
  SAMUI_ENGINE_ASSERT(data, "Failed to load image:{0}", path.string());
  width_ = width;
  height_ = height;

  if (channels == 4) {
    internal_format_ = GL_RGBA;
  } else if (channels == 3) {
    internal_format_ = GL_RGB;
  }
  SAMUI_ENGINE_ASSERT(internal_format_, "Format not supported! channels: {0}",
                      channels);

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

Ref<Texture2D> OpenGLTexture2D::Combine(
    const std::vector<Ref<Texture2D>>& textures, uint16_t cell_width,
    uint16_t cell_height, uint16_t rows, uint16_t columns) {
  SAMUI_PROFILE_FUNCTION();
  // uint16_t width = cell_width * columns;
  // uint16_t height = cell_height * rows;
  // auto     texture = std::make_shared<OpenGLTexture2D>(width, height);

  // uint32_t bpp = texture->internal_format_ == GL_RGBA ? 4 : 3;
  // SAMUI_ENGINE_ASSERT(size == width_ * height_ * bpp,
  //                     "data must be entire texture!");

  // unsigned char* texture_data =
  //     new unsigned char[cell_width * cell_height * textures.size() * bpp];
  // for (int i = 0; i < textures.size(); ++i) {
  //   // TODO: Combine

  //   // unsigned char* data =
  //   // stbi_load(textures[i].c_str(), &w, &h, &nrChannels, 0);
  //   // memcpy(texture_data + i * 16 * 16 * 3, data, 16 * 16 * 3);
  //   // if (data) {
  //   // stbi_image_free(data);
  //   // }
  // }
  // glTexImage2D(GL_TEXTURE_2D, 0 /*mipmap*/, texture->internal_format_, width,
  //              height, 0 /*legacy*/, texture->internal_format_,
  //              GL_UNSIGNED_BYTE, texture_data);
  // glGenerateMipmap(GL_TEXTURE_2D);

  return nullptr;
}

ImageInfo* OpenGLTexture2D::LoadFile(const std::filesystem::path& path) {
  SAMUI_PROFILE_FUNCTION();

  ImageInfo* info = new ImageInfo();
  stbi_set_flip_vertically_on_load(1);
  info->data = stbi_load(path.string().c_str(), &info->width, &info->height,
                         &info->channels, 0);
  return info;
}

}  // namespace samui

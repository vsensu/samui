// clang-format off
#include "opengl_frame_buffer.h"

#include <glad/glad.h>
#include <log/log.h>
// clang-format on

namespace samui {

GLenum FrameBufferTextureFormatToGL(FrameBufferTextureFormat format) {
  switch (format) {
    case FrameBufferTextureFormat::RGBA:
      return GL_RGBA;
    case FrameBufferTextureFormat::RED_INTEGER:
      return GL_RED;
    case FrameBufferTextureFormat::Depth24_Stencil8:
      return GL_DEPTH24_STENCIL8;
  }
  return GL_NONE;
}

bool IsDepthFormat(FrameBufferTextureFormat format) {
  switch (format) {
    case FrameBufferTextureFormat::Depth24_Stencil8:
      return true;
  }

  return false;
}

GLenum TextureTarget(bool multisampled) {
  return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
}

void BindTexture(bool multisampled, uint32_t texture_id) {
  glBindTexture(TextureTarget(multisampled), texture_id);
}

void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
  glGenTextures(count, outID);
}

void AttachColorTexture(uint32_t id, int samples, GLenum format, uint32_t width,
                        uint32_t height, int index) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                            height, GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, nullptr);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                         TextureTarget(multisampled), id, 0);
}

void AttachDepthTexture(uint32_t id, int samples, GLenum format,
                        GLenum attachmentType, uint32_t width,
                        uint32_t height) {
  bool multisampled = samples > 1;
  if (multisampled) {
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width,
                            height, GL_FALSE);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  }

  glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType,
                         TextureTarget(multisampled), id, 0);
}

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
    : spec_(spec) {
  for (auto attachment : spec.attachments) {
    if (!IsDepthFormat(attachment.format)) {
      color_attachments_spec_.push_back(attachment);
    } else {
      depth_attachment_spec_ = attachment;
    }
  }

  Invalidate();
}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
  glDeleteFramebuffers(1, &buffer_id_);
  glDeleteTextures(color_attachments_.size(), color_attachments_.data());
  glDeleteTextures(1, &depth_attachment_);
}

void OpenGLFrameBuffer::Invalidate() {
  if (buffer_id_) {
    glDeleteFramebuffers(1, &buffer_id_);
    glDeleteTextures(color_attachments_.size(), color_attachments_.data());
    glDeleteTextures(1, &depth_attachment_);
  }

  // 一个完整的帧缓冲需要满足以下的条件：
  // 附加至少一个缓冲（颜色、深度或模板缓冲）。
  // 至少有一个颜色附件(Attachment)。
  // 所有的附件都必须是完整的（保留了内存）。
  // 每个缓冲都应该有相同的样本数。
  glGenFramebuffers(1, &buffer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);

  bool multi_sample = spec_.samples > 1;

  if (!color_attachments_spec_.empty()) {
    color_attachments_.resize(color_attachments_spec_.size());
    CreateTextures(multi_sample, color_attachments_.data(),
                   color_attachments_.size());

    for (std::size_t i = 0; i < color_attachments_.size(); ++i) {
      BindTexture(multi_sample, color_attachments_[i]);
      auto format =
          FrameBufferTextureFormatToGL(color_attachments_spec_[i].format);
      AttachColorTexture(color_attachments_[i], spec_.samples, format,
                         spec_.width, spec_.height, i);
    }
  }

  if (depth_attachment_spec_.format != FrameBufferTextureFormat::None) {
    CreateTextures(multi_sample, &depth_attachment_, 1);
    BindTexture(multi_sample, depth_attachment_);
    auto format = FrameBufferTextureFormatToGL(depth_attachment_spec_.format);
    AttachDepthTexture(depth_attachment_, spec_.samples, format,
                       GL_DEPTH_STENCIL_ATTACHMENT, spec_.width, spec_.height);
  }

  if (color_attachments_.size() > 1) {
    SAMUI_ENGINE_ASSERT(color_attachments_.size() <= 4);
    GLenum buffers[4] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1,
                         GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
    glDrawBuffers(color_attachments_.size(), buffers);
  } else if (color_attachments_.size() == 0) {
    glDrawBuffers(0, nullptr);
  }

  SAMUI_ENGINE_ASSERT(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "FrameBuffer is incomplete!");

  // 要保证所有的渲染操作在主窗口中有视觉效果，我们需要再次激活默认帧缓冲，将它绑定到0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
  glViewport(0, 0, spec_.width, spec_.height);
}

void OpenGLFrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height) {
  constexpr uint32_t max_buffer_size = 8192;
  if (width == 0 || height == 0 || width > max_buffer_size ||
      height > max_buffer_size) {
    SAMUI_ENGINE_ERROR("OpenGLFrameBuffer::Resize: Invalid size {0},{1}!",
                       width, height);
    return;
  }

  spec_.width = width;
  spec_.height = height;
  Invalidate();
}

int OpenGLFrameBuffer::ReadPixel(uint32_t attachment_index, int x,
                                 int y) const {
  SAMUI_ENGINE_ASSERT(attachment_index < color_attachments_.size());
  glReadBuffer(GL_COLOR_ATTACHMENT0 + attachment_index);
  int pixel;
  glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixel);
  return pixel;
}

}  // namespace samui

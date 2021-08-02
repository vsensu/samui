#include "opengl_frame_buffer.h"

#include <glad/glad.h>

namespace samui {

OpenGLFrameBuffer::OpenGLFrameBuffer(const FrameBufferSpecification& spec)
    : spec_(spec) {}

OpenGLFrameBuffer::~OpenGLFrameBuffer() {
  glDeleteFramebuffers(1, &buffer_id_);
}

void OpenGLFrameBuffer::Invalidate() {
  // 一个完整的帧缓冲需要满足以下的条件：
  // 附加至少一个缓冲（颜色、深度或模板缓冲）。
  // 至少有一个颜色附件(Attachment)。
  // 所有的附件都必须是完整的（保留了内存）。
  // 每个缓冲都应该有相同的样本数。
  glGenFramebuffers(1, &buffer_id_);
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);

  // 为帧缓冲创建一个纹理和创建一个普通的纹理差不多，主要的区别就是，
  // 我们将维度设置为了屏幕大小（尽管这不是必须的），并且我们给纹理的data参数传递了NULL。
  // 对于这个纹理，我们仅仅分配了内存而没有填充它。填充这个纹理将会在我们渲染到帧缓冲之后来进行。
  // 同样注意我们并不关心环绕方式或多级渐远纹理，我们在大多数情况下都不会需要它们。
  glGenTextures(1, &texture_);
  glBindTexture(GL_TEXTURE_2D, texture_);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, spec_.width, spec_.height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // 现在我们已经创建好一个纹理了，要做的最后一件事就是将它附加到帧缓冲上了
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         texture_, 0);

  glGenTextures(1, &depth_);
  glBindTexture(GL_TEXTURE_2D, depth_);
  glTexImage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, spec_.width, spec_.height,
               0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                         GL_TEXTURE_2D, depth_, 0);

  SAMUI_ENGINE_ASSERT(
      glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE,
      "FrameBuffer is incomplete!");

  // 要保证所有的渲染操作在主窗口中有视觉效果，我们需要再次激活默认帧缓冲，将它绑定到0
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLFrameBuffer::Bind() {
  glBindFramebuffer(GL_FRAMEBUFFER, buffer_id_);
}

void OpenGLFrameBuffer::Unbind() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

}  // namespace samui

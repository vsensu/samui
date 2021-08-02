#ifndef SAMUI_OPENGL_FRAME_BUFFER_H_
#define SAMUI_OPENGL_FRAME_BUFFER_H_

#include "frame_buffer.h"

namespace samui {
class SAMUI_API OpenGLFrameBuffer : public FrameBuffer {
 public:
  OpenGLFrameBuffer(const FrameBufferSpecification& spec);
  virtual ~OpenGLFrameBuffer();

  void Invalidate();

  virtual void     Bind() override;
  virtual void     Unbind() override;
  virtual uint32_t GetColorAttachmentRenderID() const override {
    return texture_;
  }
  virtual const FrameBufferSpecification& GetSpecification() const override {
    return spec_;
  }

 private:
  uint32_t                 buffer_id_;
  uint32_t                 texture_, depth_;
  FrameBufferSpecification spec_;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_FRAME_BUFFER_H_
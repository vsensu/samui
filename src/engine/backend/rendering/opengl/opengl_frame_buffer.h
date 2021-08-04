#ifndef SAMUI_OPENGL_FRAME_BUFFER_H_
#define SAMUI_OPENGL_FRAME_BUFFER_H_

// clang-format off
#include <rendering/frame_buffer.h>
//clang-format on

namespace samui {
class SAMUI_API OpenGLFrameBuffer : public FrameBuffer {
 public:
  OpenGLFrameBuffer(const FrameBufferSpecification& spec);
  virtual ~OpenGLFrameBuffer();

  void Invalidate();

  virtual void     Bind() override;
  virtual void     Unbind() override;
  virtual void     Resize(uint32_t width, uint32_t height) override;
  virtual uint32_t GetColorAttachmentRenderID() const override {
    return texture_;
  }
  virtual const FrameBufferSpecification& GetSpecification() const override {
    return spec_;
  }

 private:
  uint32_t                 buffer_id_{0};
  uint32_t                 texture_{0}, depth_{0};
  FrameBufferSpecification spec_;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_FRAME_BUFFER_H_
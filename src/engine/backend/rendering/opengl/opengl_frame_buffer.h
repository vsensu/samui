#ifndef SAMUI_OPENGL_FRAME_BUFFER_H_
#define SAMUI_OPENGL_FRAME_BUFFER_H_

// clang-format off
#include <vector>

#include <core/core.h>
#include <log/log.h>
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

  virtual int      ReadPixel(uint32_t attachment_index, int x, int y) const override;
  virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0) const override {
    SAMUI_ENGINE_ASSERT(index < color_attachments_.size());
    return color_attachments_[index];
  }

  virtual uint32_t ColorAttachmentCount() const override {
    return color_attachments_.size();
  }

  virtual const FrameBufferSpecification& GetSpecification() const override {
    return spec_;
  }

 private:
  uint32_t                 buffer_id_{0};
  FrameBufferSpecification spec_;

  std::vector<FrameBufferTextureSpecification> color_attachments_spec_;
  FrameBufferTextureSpecification depth_attachment_spec_;
  
  std::vector<uint32_t> color_attachments_;
  uint32_t depth_attachment_;
};
}  // namespace samui

#endif  // SAMUI_OPENGL_FRAME_BUFFER_H_
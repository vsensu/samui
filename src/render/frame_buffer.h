#ifndef SAMUI_FRAME_BUFFER_H_
#define SAMUI_FRAME_BUFFER_H_

#include "../core/core.h"

namespace samui {
struct SAMUI_API FrameBufferSpecification {
  uint32_t width, height;
  uint32_t samples = 1;
  bool     swap_chain_target = false;
};

class SAMUI_API FrameBuffer {
 public:
  virtual ~FrameBuffer() {}
  virtual void                            Bind() = 0;
  virtual void                            Unbind() = 0;
  virtual uint32_t                        GetColorAttachmentRenderID() const = 0;
  virtual const FrameBufferSpecification& GetSpecification() const = 0;

  static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};
}  // namespace samui

#endif  // SAMUI_FRAME_BUFFER_H_
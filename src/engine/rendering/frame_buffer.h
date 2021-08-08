#ifndef SAMUI_FRAME_BUFFER_H_
#define SAMUI_FRAME_BUFFER_H_

// clang-format off
#include <vector>

#include <core/core.h>
// clang-format on

namespace samui {
enum class FrameBufferTextureFormat {
  None = 0,
  RGBA,
  Depth24_Stencil8,
  Depth = Depth24_Stencil8
};

struct SAMUI_API FrameBufferTextureSpecification {
  FrameBufferTextureSpecification() = default;
  FrameBufferTextureSpecification(FrameBufferTextureFormat format_)
      : format(format_) {}
  FrameBufferTextureFormat format{FrameBufferTextureFormat::None};
  // TODO: filtering/warp
};

struct SAMUI_API FrameBufferSpecification {
  uint32_t                                     width, height;
  std::vector<FrameBufferTextureSpecification> attachments;
  uint32_t                                     samples = 1;
  bool                                         swap_chain_target = false;
};

class SAMUI_API FrameBuffer {
 public:
  virtual ~FrameBuffer() {}
  virtual void     Bind() = 0;
  virtual void     Unbind() = 0;
  virtual void     Resize(uint32_t width, uint32_t height) = 0;
  virtual uint32_t GetColorAttachmentRenderID(uint32_t index = 0) const = 0;
  virtual uint32_t ColorAttachmentCount() const = 0;
  virtual const FrameBufferSpecification& GetSpecification() const = 0;

  static Ref<FrameBuffer> Create(const FrameBufferSpecification& spec);
};
}  // namespace samui

#endif  // SAMUI_FRAME_BUFFER_H_
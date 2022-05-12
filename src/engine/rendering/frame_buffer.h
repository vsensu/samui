#pragma once

// clang-format off
#include <vector>
#include <memory>

#include <core/core.h>
// clang-format on

namespace samui
{
enum class FrameBufferTextureFormat
{
    None = 0,
    RGBA,
    RED_INTEGER,
    Depth24_Stencil8,
    Depth = Depth24_Stencil8
};

struct SAMUI_API FrameBufferTextureSpecification
{
    FrameBufferTextureSpecification() = default;
    FrameBufferTextureSpecification(FrameBufferTextureFormat format_)
        : format(format_)
    {
    }
    FrameBufferTextureFormat format{FrameBufferTextureFormat::None};
    // TODO: filtering/warp
};

struct SAMUI_API FrameBufferSpecification
{
    uint32_t                                     width, height;
    std::vector<FrameBufferTextureSpecification> attachments;
    uint32_t                                     samples = 1;
    bool                                         swap_chain_target = false;
};

class SAMUI_API FrameBuffer
{
  public:
    virtual ~FrameBuffer() {}
    virtual void     bind() = 0;
    virtual void     unbind() = 0;
    virtual void     resize(uint32_t width, uint32_t height) = 0;
    virtual uint32_t read_pixel(uint32_t attachment_index, int x,
                                int y) const = 0;
    virtual uint32_t get_color_attachment_render_id(
        uint32_t index = 0) const = 0;
    virtual uint32_t                        color_attachment_count() const = 0;
    virtual const FrameBufferSpecification& get_specification() const = 0;
    virtual void clear_attachment(uint32_t attachment_index,
                                  uint32_t value) = 0;

    static std::shared_ptr<FrameBuffer> create(
        const FrameBufferSpecification& spec);
};
}  // namespace samui

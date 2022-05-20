#pragma once

// clang-format off
#include <vector>

#include <rendering/frame_buffer.h>

#include <log/log.h>
#include <assert/assert.h>

#include "opengl_module.h"
// clang-format on

namespace samui
{
class SAMUI_OPENGL_API OpenGLFrameBuffer : public FrameBuffer
{
  public:
    OpenGLFrameBuffer(const FrameBufferSpecification& spec);
    virtual ~OpenGLFrameBuffer();

    void invalidate();

    virtual void bind() override;
    virtual void unbind() override;
    virtual void resize(uint32_t width, uint32_t height) override;

    virtual uint32_t read_pixel(uint32_t attachment_index, int x,
                                int y) const override;
    virtual uint32_t get_color_attachment_render_id(
        uint32_t index = 0) const override
    {
        SAMUI_ENGINE_ASSERT(index < color_attachments_.size());
        return color_attachments_[index];
    }

    virtual uint32_t color_attachment_count() const override
    {
        return color_attachments_.size();
    }

    virtual const FrameBufferSpecification& get_specification() const override
    {
        return spec_;
    }

    virtual void clear_attachment(uint32_t attachment_index,
                                  uint32_t value) override;

  private:
    uint32_t                 buffer_id_{0};
    FrameBufferSpecification spec_;

    std::vector<FrameBufferTextureSpecification> color_attachments_spec_;
    FrameBufferTextureSpecification              depth_attachment_spec_;

    std::vector<uint32_t> color_attachments_;
    uint32_t              depth_attachment_;
};
}  // namespace samui

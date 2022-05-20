#pragma once

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <memory>

#include <rendering/buffer.h>

#include "opengl_module.h"
// clang-format on

namespace samui
{
class SAMUI_OPENGL_API OpenGLVertexBuffer : public VertexBuffer
{
  public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(const void* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();
    void bind() override;
    void unbind() override;

    virtual void set_data(const void* data, uint32_t size) override;

    const BufferLayout& get_layout() const override { return layout_; }
    void set_layout(const BufferLayout& layout) override { layout_ = layout; }

  private:
    uint32_t     buffer_id_{0};
    BufferLayout layout_;
};

class SAMUI_OPENGL_API OpenGLIndexBuffer : public IndexBuffer
{
  public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t size);
    virtual ~OpenGLIndexBuffer();
    uint32_t get_count() const override { return count_; }
    void     bind() override;
    void     unbind() override;

  private:
    uint32_t buffer_id_{0};
    uint32_t count_{0};
};

class SAMUI_OPENGL_API OpenGLVertexArray : public VertexArray
{
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void bind() override;
    void unbind() override;

    void add_vertex_buffer(const std::shared_ptr<VertexBuffer>&) override;
    void set_index_buffer(const std::shared_ptr<IndexBuffer>&) override;
    IndexBuffer* get_index_buffer() const override
    {
        return index_buffer_.get();
    }

  private:
    std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
    std::shared_ptr<IndexBuffer>               index_buffer_;
    uint32_t                                   vao_id_{0};
};

}  // namespace samui
#endif

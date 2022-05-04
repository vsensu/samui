#pragma once

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <memory>

#include <core/core.h>
#include <rendering/buffer.h>
// clang-format on

namespace samui
{
class SAMUI_API OpenGLVertexBuffer : public VertexBuffer
{
  public:
    OpenGLVertexBuffer(uint32_t size);
    OpenGLVertexBuffer(const void* vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();
    void Bind() override;
    void UnBind() override;

    virtual void SetData(const void* data, uint32_t size) override;

    const BufferLayout& GetLayout() const override { return layout_; }
    void SetLayout(const BufferLayout& layout) override { layout_ = layout; }

  private:
    uint32_t     buffer_id_{0};
    BufferLayout layout_;
};

class SAMUI_API OpenGLIndexBuffer : public IndexBuffer
{
  public:
    OpenGLIndexBuffer(const uint32_t* indices, uint32_t size);
    virtual ~OpenGLIndexBuffer();
    uint32_t GetCount() const { return count_; }
    void     Bind() override;
    void     UnBind() override;

  private:
    uint32_t buffer_id_{0};
    uint32_t count_{0};
};

class SAMUI_API OpenGLVertexArray : public VertexArray
{
  public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void Bind() override;
    void UnBind() override;

    void         AddVertexBuffer(const std::shared_ptr<VertexBuffer>&) override;
    void         SetIndexBuffer(const std::shared_ptr<IndexBuffer>&) override;
    IndexBuffer* GetIndexBuffer() const override { return index_buffer_.get(); }

  private:
    std::vector<std::shared_ptr<VertexBuffer>> vertex_buffers_;
    std::shared_ptr<IndexBuffer>               index_buffer_;
    uint32_t                                   vao_id_{0};
};

}  // namespace samui
#endif

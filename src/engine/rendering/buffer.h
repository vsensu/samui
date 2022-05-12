#pragma once

// clang-format off
#include <cstdint>
#include <memory>

#include <core/core.h>
#include <log/log.h>
// clang-format on

namespace samui
{

// clang-format off
enum class ShaderDataType {
  None= 0,
  Float, Float2, Float3, Float4,
  Mat3, Mat4,
  Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4,
  Bool
};

constexpr uint32_t ShaderDataTypeCount(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:   return 1;
    case ShaderDataType::Float2:  return 2;
    case ShaderDataType::Float3:  return 3;
    case ShaderDataType::Float4:  return 4;
    case ShaderDataType::Mat3:    return 3 * 3;
    case ShaderDataType::Mat4:    return 4 * 4;
    case ShaderDataType::Int:     return 1;
    case ShaderDataType::Int2:    return 2;
    case ShaderDataType::Int3:    return 3;
    case ShaderDataType::Int4:    return 4;
    case ShaderDataType::UInt:     return 1;
    case ShaderDataType::UInt2:    return 2;
    case ShaderDataType::UInt3:    return 3;
    case ShaderDataType::UInt4:    return 4;
    case ShaderDataType::Bool:    return 1;
  }
}

constexpr uint32_t ShaderDataTypeSize(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:   return 4;
    case ShaderDataType::Float2:  return 4 * 2;
    case ShaderDataType::Float3:  return 4 * 3;
    case ShaderDataType::Float4:  return 4 * 4;
    case ShaderDataType::Mat3:    return 4 * 3 * 3;
    case ShaderDataType::Mat4:    return 4 * 4 * 4;
    case ShaderDataType::Int:     return 4;
    case ShaderDataType::Int2:    return 4 * 2;
    case ShaderDataType::Int3:    return 4 * 3;
    case ShaderDataType::Int4:    return 4 * 4;
    case ShaderDataType::UInt:     return 4;
    case ShaderDataType::UInt2:    return 4 * 2;
    case ShaderDataType::UInt3:    return 4 * 3;
    case ShaderDataType::UInt4:    return 4 * 4;
    case ShaderDataType::Bool:    return 1;
  }

  SAMUI_ENGINE_ASSERT(false, "Unknown ShaderDataType");
  return 0;
}
// clang-format on

struct BufferElement
{
    std::string    Name;
    ShaderDataType Type;
    uint32_t       Size;
    uint32_t       Offset;
    bool           Normalized;

    BufferElement(const std::string& name, ShaderDataType type,
                  bool normalized = false)
        : Name(name),
          Type(type),
          Size(ShaderDataTypeSize(type)),
          Offset(0),
          Normalized(normalized)
    {
    }
};

class BufferLayout
{
  public:
    BufferLayout() {}
    BufferLayout(const std::initializer_list<BufferElement>& elements)
        : elements_(elements)
    {
        calc_stride_and_offset();
    }

    inline uint32_t get_stride() const { return stride_; }

    inline const std::vector<BufferElement>& get_elements() const
    {
        return elements_;
    }

    std::vector<BufferElement>::iterator begin() { return elements_.begin(); }
    std::vector<BufferElement>::iterator end() { return elements_.end(); }
    std::vector<BufferElement>::const_iterator begin() const
    {
        return elements_.begin();
    }
    std::vector<BufferElement>::const_iterator end() const
    {
        return elements_.end();
    }

  private:
    void calc_stride_and_offset()
    {
        stride_ = 0;
        uint32_t offset = 0;
        for (auto& elem : elements_)
        {
            elem.Offset = offset;
            offset += elem.Size;
            stride_ += elem.Size;
        }
    }

  private:
    std::vector<BufferElement> elements_;
    uint32_t                   stride_;
};

class SAMUI_API Buffer
{
  public:
    virtual ~Buffer() {}

    virtual void bind() = 0;
    virtual void unbind() = 0;
};

class SAMUI_API VertexBuffer : public Buffer
{
  public:
    virtual ~VertexBuffer() {}

    virtual const BufferLayout& get_layout() const = 0;
    virtual void                set_layout(const BufferLayout& layout) = 0;

    virtual void set_data(const void* data, uint32_t size) = 0;

    static std::shared_ptr<VertexBuffer> create(uint32_t size);
    static std::shared_ptr<VertexBuffer> create(const void* vertices,
                                                uint32_t    size);
};

class SAMUI_API IndexBuffer : public Buffer
{
  public:
    virtual ~IndexBuffer() {}

    virtual uint32_t get_count() const = 0;

    static std::shared_ptr<IndexBuffer> create(const uint32_t* indices,
                                               uint32_t        count);
};

class SAMUI_API VertexArray
{
  public:
    virtual ~VertexArray() {}

    virtual void bind() = 0;
    virtual void unbind() = 0;

    virtual void add_vertex_buffer(
        const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
    virtual void set_index_buffer(
        const std::shared_ptr<IndexBuffer>& vertexBuffer) = 0;
    virtual IndexBuffer* get_index_buffer() const = 0;

    static std::shared_ptr<VertexArray> create();
};

}  // namespace samui

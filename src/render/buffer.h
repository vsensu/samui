#ifndef SAMUI_BUFFER_H_
#define SAMUI_BUFFER_H_

#include <cstdint>

#include "core/core.h"
#include "log/log.h"

namespace samui {

// clang-format off
enum class ShaderDataType {
  None= 0,
  Float, Float2, Float3, Float4,
  Mat3, Mat4,
  Int, Int2, Int3, Int4,
  Bool
};

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
  case ShaderDataType::Bool:    return 1;
  }

  SAMUI_ENGINE_ASSERT(false, "Unknown ShaderDataType");
  return 0;
}
// clang-format on

struct BufferElement {
  std::string    Name;
  ShaderDataType Type;
  uint32_t       Size;
  uint32_t       Offset;

  BufferElement(const std::string& name, ShaderDataType type)
      : Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0) {}
};

class BufferLayout {
 public:
  BufferLayout(const std::initializer_list<BufferElement>& elements)
      : elements_(elements) {
    CalcStrideAndOffset();
  }

  inline const std::vector<BufferElement>& GetElements() const {
    return elements_;
  }

 private:
  void CalcStrideAndOffset() {
    stride_ = 0;
    uint32_t offset = 0;
    for (auto& elem : elements_) {
      elem.Offset = offset;
      offset += elem.Size;
      stride_ += elem.Size;
    }
  }

 private:
  std::vector<BufferElement> elements_;
  uint32_t                   stride_;
};

class SAMUI_API Buffer {
 public:
  virtual ~Buffer() {}

  virtual void Bind() = 0;
  virtual void UnBind() = 0;
};

class SAMUI_API VertexBuffer : public Buffer {
 public:
  virtual ~VertexBuffer() {}

  // virtual void SetLayout(const BufferLayout &layout) = 0;

  static VertexBuffer* Create(float* vertices, uint32_t size);
};

class SAMUI_API IndexBuffer : public Buffer {
 public:
  virtual ~IndexBuffer() {}

  virtual uint32_t GetCount() const = 0;

  static IndexBuffer* Create(uint32_t* indices, uint32_t count);
};
}  // namespace samui

#endif
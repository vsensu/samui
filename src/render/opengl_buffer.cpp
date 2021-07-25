#ifdef SAMUI_RENDER_OPENGL

#include "opengl_buffer.h"

#include <glad/glad.h>

namespace samui {
// clang-format off
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:   return GL_FLOAT;
    case ShaderDataType::Float2:  return GL_FLOAT;
    case ShaderDataType::Float3:  return GL_FLOAT;
    case ShaderDataType::Float4:  return GL_FLOAT;
    case ShaderDataType::Mat3:    return GL_FLOAT;
    case ShaderDataType::Mat4:    return GL_FLOAT;
    case ShaderDataType::Int:     return GL_INT;
    case ShaderDataType::Int2:    return GL_INT;
    case ShaderDataType::Int3:    return GL_INT;
    case ShaderDataType::Int4:    return GL_INT;
    case ShaderDataType::Bool:    return GL_BOOL;
  }

  SAMUI_ENGINE_ASSERT(false, "Unkown ShaderDataType");
  return 0;
}
// clang-format on

// VertexBuffer-------------------------------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  glGenBuffers(1, &buffer_id_);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &buffer_id_); }

void OpenGLVertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, buffer_id_); }

void OpenGLVertexBuffer::UnBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// IndexBuffer-------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  glGenBuffers(1, &buffer_id_);
  Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &buffer_id_); }

void OpenGLIndexBuffer::Bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

void OpenGLIndexBuffer::UnBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }

OpenGLVertexArray::OpenGLVertexArray() {
  glGenVertexArrays(1, &vao_id_);
  Bind();
}

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &vao_id_); }

void OpenGLVertexArray::Bind() { glBindVertexArray(vao_id_); }

void OpenGLVertexArray::UnBind() { glBindVertexArray(0); }

void OpenGLVertexArray::AddVertexBuffer(
    const samui::Ref<VertexBuffer>& buffer) {
  SAMUI_ENGINE_ASSERT(buffer->GetLayout().GetElements().size(),
                      "Vertex Buffer has no layout!");

  Bind();
  buffer->Bind();

  uint32_t    index = 0;
  const auto& layout = buffer->GetLayout();
  for (const auto& elem : layout) {
    // 定义OpenGL如何理解该顶点数据
    glVertexAttribPointer(index, ShaderDataTypeCount(elem.Type),
                          ShaderDataTypeToOpenGLBaseType(elem.Type),
                          elem.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(), (const void*)elem.Offset);

    // 启用顶点属性 顶点属性默认是禁用的
    glEnableVertexAttribArray(index);
    index++;
  }

  vertex_buffers_.push_back(buffer);
}

void OpenGLVertexArray::SetIndexBuffer(
    const samui::Ref<IndexBuffer>& buffer) {
  Bind();
  buffer->Bind();
  index_buffer_ = buffer;
}

}  // namespace samui

#endif
#ifdef SAMUI_RENDER_OPENGL

#include "opengl_buffer.h"

#include <glad/glad.h>

#include "../debug/instrumentor.h"

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
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size) {
  SAMUI_PROFILE_FUNCTION();
  glGenBuffers(1, &buffer_id_);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  SAMUI_PROFILE_FUNCTION();
  glGenBuffers(1, &buffer_id_);
  Bind();
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
  SAMUI_PROFILE_FUNCTION();
  glDeleteBuffers(1, &buffer_id_);
}

void OpenGLVertexBuffer::Bind() {
  SAMUI_PROFILE_FUNCTION();
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
}

void OpenGLVertexBuffer::UnBind() {
  SAMUI_PROFILE_FUNCTION();
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::SetData(const void* data, uint32_t size) {
  SAMUI_PROFILE_FUNCTION();
  glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
  glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

// IndexBuffer-------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  SAMUI_PROFILE_FUNCTION();
  glGenBuffers(1, &buffer_id_);
  Bind();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
  SAMUI_PROFILE_FUNCTION();
  glDeleteBuffers(1, &buffer_id_);
}

void OpenGLIndexBuffer::Bind() {
  SAMUI_PROFILE_FUNCTION();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

void OpenGLIndexBuffer::UnBind() {
  SAMUI_PROFILE_FUNCTION();
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLVertexArray::OpenGLVertexArray() {
  SAMUI_PROFILE_FUNCTION();
  glGenVertexArrays(1, &vao_id_);
  Bind();
}

OpenGLVertexArray::~OpenGLVertexArray() {
  SAMUI_PROFILE_FUNCTION();
  glDeleteVertexArrays(1, &vao_id_);
}

void OpenGLVertexArray::Bind() {
  SAMUI_PROFILE_FUNCTION();
  glBindVertexArray(vao_id_);
}

void OpenGLVertexArray::UnBind() {
  SAMUI_PROFILE_FUNCTION();
  glBindVertexArray(0);
}

void OpenGLVertexArray::AddVertexBuffer(
    const samui::Ref<VertexBuffer>& buffer) {
  SAMUI_PROFILE_FUNCTION();
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

void OpenGLVertexArray::SetIndexBuffer(const samui::Ref<IndexBuffer>& buffer) {
  SAMUI_PROFILE_FUNCTION();
  Bind();
  buffer->Bind();
  index_buffer_ = buffer;
}

}  // namespace samui

#endif
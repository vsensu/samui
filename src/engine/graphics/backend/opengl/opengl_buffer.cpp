#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include "opengl_buffer.h"

#include <glad/glad.h>

#include <assert/assert.h>
#include <profiler/instrumentor.h>
// clang-format on

namespace samui
{
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
    case ShaderDataType::UInt:    return GL_UNSIGNED_INT;
    case ShaderDataType::UInt2:    return GL_UNSIGNED_INT;
    case ShaderDataType::UInt3:    return GL_UNSIGNED_INT;
    case ShaderDataType::UInt4:    return GL_UNSIGNED_INT;
    case ShaderDataType::Bool:    return GL_BOOL;
  }

  SAMUI_ENGINE_ASSERT(false, "Unkown ShaderDataType");
  return 0;
}
// clang-format on

// VertexBuffer-------------------------------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size)
{
    SAMUI_PROFILE_FUNCTION();
    glGenBuffers(1, &buffer_id_);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* vertices, uint32_t size)
{
    SAMUI_PROFILE_FUNCTION();
    glGenBuffers(1, &buffer_id_);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    SAMUI_PROFILE_FUNCTION();
    glDeleteBuffers(1, &buffer_id_);
}

void OpenGLVertexBuffer::bind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
}

void OpenGLVertexBuffer::unbind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLVertexBuffer::set_data(const void* data, uint32_t size)
{
    SAMUI_PROFILE_FUNCTION();
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

// IndexBuffer-------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t count)
    : count_(count)
{
    SAMUI_PROFILE_FUNCTION();
    glGenBuffers(1, &buffer_id_);
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
                 GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    SAMUI_PROFILE_FUNCTION();
    glDeleteBuffers(1, &buffer_id_);
}

void OpenGLIndexBuffer::bind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

void OpenGLIndexBuffer::unbind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

OpenGLVertexArray::OpenGLVertexArray()
{
    SAMUI_PROFILE_FUNCTION();
    glGenVertexArrays(1, &vao_id_);
    bind();
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    SAMUI_PROFILE_FUNCTION();
    glDeleteVertexArrays(1, &vao_id_);
}

void OpenGLVertexArray::bind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindVertexArray(vao_id_);
}

void OpenGLVertexArray::unbind()
{
    SAMUI_PROFILE_FUNCTION();
    glBindVertexArray(0);
}

void OpenGLVertexArray::add_vertex_buffer(
    const std::shared_ptr<VertexBuffer>& buffer)
{
    SAMUI_PROFILE_FUNCTION();
    SAMUI_ENGINE_ASSERT(buffer->get_layout().get_elements().size(),
                        "Vertex Buffer has no layout!");

    bind();
    buffer->bind();

    uint32_t    index = 0;
    const auto& layout = buffer->get_layout();
    for (const auto& elem : layout)
    {
        switch (elem.Type)
        {
            case ShaderDataType::Bool:
            case ShaderDataType::Int:
            case ShaderDataType::Int2:
            case ShaderDataType::Int3:
            case ShaderDataType::Int4:
            case ShaderDataType::UInt:
            case ShaderDataType::UInt2:
            case ShaderDataType::UInt3:
            case ShaderDataType::UInt4:
                // define how OpenGL should interpret this vertex data
                glVertexAttribIPointer(
                    index, ShaderDataTypeCount(elem.Type),
                    ShaderDataTypeToOpenGLBaseType(elem.Type),
                    layout.get_stride(), (const void*)elem.Offset);
                break;
            default:
                // define how OpenGL should interpret this vertex data
                glVertexAttribPointer(index, ShaderDataTypeCount(elem.Type),
                                      ShaderDataTypeToOpenGLBaseType(elem.Type),
                                      elem.Normalized ? GL_TRUE : GL_FALSE,
                                      layout.get_stride(),
                                      (const void*)elem.Offset);

                break;
        }

        // enable vertex attribute, vertex attribute is disabled by default
        glEnableVertexAttribArray(index);
        index++;
    }

    vertex_buffers_.push_back(buffer);
}

void OpenGLVertexArray::set_index_buffer(
    const std::shared_ptr<IndexBuffer>& buffer)
{
    SAMUI_PROFILE_FUNCTION();
    bind();
    buffer->bind();
    index_buffer_ = buffer;
}

}  // namespace samui

#endif
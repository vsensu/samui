#ifdef SAMUI_RENDER_OPENGL

#include "opengl_buffer.h"

#include <glad/glad.h>

namespace samui {
// VertexBuffer-------------------------------------------------------------------
OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size) {
  glGenBuffers(1, &buffer_id_);
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &buffer_id_); }

void OpenGLVertexBuffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, buffer_id_); }

void OpenGLVertexBuffer::UnBind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

// IndexBuffer-------------------------------------------------------------------
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
    : count_(count) {
  glGenBuffers(1, &buffer_id_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices,
               GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &buffer_id_); }

void OpenGLIndexBuffer::Bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer_id_);
}

void OpenGLIndexBuffer::UnBind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
}  // namespace samui

#endif
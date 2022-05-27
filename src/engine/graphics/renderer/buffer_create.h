#pragma once

// clang-format off
#include <memory>

#include <rendering/buffer.h>
#include <rendering/frame_buffer.h>

#include "renderer_module.h"
// clang-format on

namespace samui
{
namespace vertex_buffer
{
SAMUI_RENDERER_API std::shared_ptr<VertexBuffer> create(uint32_t size);
SAMUI_RENDERER_API std::shared_ptr<VertexBuffer> create(const void* vertices, uint32_t size);
}  // namespace vertex_buffer

namespace index_buffer
{
SAMUI_RENDERER_API std::shared_ptr<IndexBuffer> create(const uint32_t* indices, uint32_t count);
}  // namespace index_buffer

namespace vertex_array
{
SAMUI_RENDERER_API std::shared_ptr<VertexArray> create();

}  // namespace vertex_array

namespace frame_buffer
{
SAMUI_RENDERER_API std::shared_ptr<FrameBuffer> create(const FrameBufferSpecification& spec);
}
}  // namespace samui

#pragma once

// clang-format off
#include <memory>

#include <rendering/buffer.h>

#include "renderer_module.h"
// clang-format on

namespace samui
{
namespace vertex_buffer
{
std::shared_ptr<VertexBuffer> create(uint32_t size);
std::shared_ptr<VertexBuffer> create(const void* vertices,
                                            uint32_t    size);
}  // namespace vertex_buffer

namespace index_buffer
{
std::shared_ptr<IndexBuffer> create(const uint32_t* indices,
                                           uint32_t        count);
}  // namespace index_buffer

namespace vertex_array
{
std::shared_ptr<VertexArray> create();

}  // namespace vertex_array
}  // namespace samui

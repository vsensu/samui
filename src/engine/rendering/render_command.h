#pragma once

#include "renderer_api.h"

namespace samui
{
class SAMUI_API RenderCommand
{
  public:
    inline static void init() { renderer_api_->init(); }

    inline static void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                    uint32_t height)
    {
        renderer_api_->set_viewport(x, y, width, height);
    }

    inline static void set_clear_color(const glm::vec4& color)
    {
        renderer_api_->set_clear_color(color);
    }
    inline static void clear() { renderer_api_->clear(); }

    inline static void draw_indexed(
        const std::shared_ptr<VertexArray>& vertex_array,
        uint32_t                            index_count = 0)
    {
        vertex_array->bind();
        renderer_api_->draw_indexed(vertex_array, index_count);
    }

    inline static void set_depth_test_enable(bool enable)
    {
        renderer_api_->set_depth_test_enable(enable);
    }

    inline static void set_depth_test_func(DepthFunc depth_func)
    {
        renderer_api_->set_depth_test_func(depth_func);
    }

    inline static void set_cull_face_enable(bool enable)
    {
        renderer_api_->set_cull_face_enable(enable);
    }

    inline static void set_blend_enable(bool enable)
    {
        renderer_api_->set_blend_enable(enable);
    }

    inline static void set_blend_func(BlendFactor src_factor,
                                      BlendFactor dest_factor)
    {
        renderer_api_->set_blend_func(src_factor, dest_factor);
    }

    inline static void set_cull_face(CullFaceType cull_face)
    {
        renderer_api_->set_cull_face(cull_face);
    }

    inline static void set_front_face(FaceVertexOrder face_vertex_order)
    {
        renderer_api_->set_front_face(face_vertex_order);
    }

    inline static void set_polygon_mode(PolygonMode polygon_mode)
    {
        renderer_api_->set_polygon_mode(polygon_mode);
    }

    inline static void set_flip_vertically_on_load(bool flip_vertically)
    {
        renderer_api_->set_flip_vertically_on_load(flip_vertically);
    }

  private:
    static RendererAPI* renderer_api_;
};
}  // namespace samui

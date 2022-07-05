#pragma once

// clang-format off
#include <rendering/buffer.h>
#include <rendering/rendering_api.h>

#include "renderer_module.h"
// clang-format on

namespace samui
{

class SAMUI_RENDERER_API RenderCommand
{
  public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

    static API get_api();

  private:
    static API api_;

  public:
    static void init();
    static void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                             uint32_t height);

    static void set_clear_color(const glm::vec4& color);
    static void clear();

    static void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array,
                             uint32_t index_count = 0);

    static void set_depth_test_enable(bool enable);

    static void set_depth_test_func(DepthFunc depth_func);

    static void set_cull_face_enable(bool enable);

    static void set_blend_enable(bool enable);

    static void set_blend_func(BlendFactor src_factor, BlendFactor dest_factor);

    static void set_blend_func_separate(BlendFactor src_rgb, BlendFactor dst_rgb,
                                 BlendFactor src_alpha, BlendFactor dst_alpha);

    static void set_cull_face(CullFaceType cull_face);

    static void set_front_face(FaceVertexOrder face_vertex_order);

    static void set_polygon_mode(PolygonMode polygon_mode);

    static void set_flip_vertically_on_load(bool flip_vertically);

  private:
    static RenderingAPI* rendering_api_;
};

}  // namespace samui

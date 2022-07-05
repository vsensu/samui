#pragma once

#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include <memory>

#include <rendering/rendering_api.h>

#include "opengl_module.h"
// clang-format on

namespace samui
{
class SAMUI_OPENGL_API OpenGLRenderingAPI : public RenderingAPI
{
  public:
    void         init() override;
    void         set_viewport(uint32_t x, uint32_t y, uint32_t width,
                              uint32_t height) override;
    void         set_clear_color(const glm::vec4& color) override;
    void         clear() override;
    void         draw_indexed(const std::shared_ptr<VertexArray>& vertex_array,
                              uint32_t index_count = 0) override;
    virtual void set_depth_test_enable(bool enable) override;
    virtual void set_depth_test_func(DepthFunc depth_func) override;
    virtual void set_cull_face_enable(bool enable) override;
    virtual void set_blend_enable(bool enable) override;
    virtual void set_blend_func(BlendFactor src_factor,
                                BlendFactor dest_factor) override;
    virtual void set_blend_func_separate(BlendFactor src_rgb,
                                         BlendFactor dst_rgb,
                                         BlendFactor src_alpha,
                                         BlendFactor dst_alpha) override;
    virtual void set_cull_face(CullFaceType cull_face) override;
    virtual void set_front_face(FaceVertexOrder face_vertex_order) override;
    virtual void set_polygon_mode(PolygonMode polygon_mode) override;
    virtual void set_flip_vertically_on_load(bool flip_vertically) override;
};
}  // namespace samui

#endif
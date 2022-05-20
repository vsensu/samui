// clang-format off
#include "render_command.h"

#ifdef SAMUI_RENDER_OPENGL
#include <backend/rendering/opengl/opengl_rendering_api.h>
#endif

// clang-format on

namespace samui
{
#ifdef SAMUI_RENDER_OPENGL
RenderingAPI* RenderCommand::rendering_api_ = new OpenGLRenderingAPI();
#endif

#ifdef SAMUI_RENDER_OPENGL
RenderCommand::API RenderCommand::api_ = RenderCommand::API::OpenGL;
#endif

RenderCommand::API RenderCommand::get_api() { return api_; }

void RenderCommand::init() { rendering_api_->init(); }

void RenderCommand::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                 uint32_t height)
{
    rendering_api_->set_viewport(x, y, width, height);
}

void RenderCommand::set_clear_color(const glm::vec4& color)
{
    rendering_api_->set_clear_color(color);
}

void RenderCommand::clear() { rendering_api_->clear(); }

void RenderCommand::draw_indexed(
    const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count)
{
    vertex_array->bind();
    rendering_api_->draw_indexed(vertex_array, index_count);
}

void RenderCommand::set_depth_test_enable(bool enable)
{
    rendering_api_->set_depth_test_enable(enable);
}

void RenderCommand::set_depth_test_func(DepthFunc depth_func)
{
    rendering_api_->set_depth_test_func(depth_func);
}

void RenderCommand::set_cull_face_enable(bool enable)
{
    rendering_api_->set_cull_face_enable(enable);
}

void RenderCommand::set_blend_enable(bool enable)
{
    rendering_api_->set_blend_enable(enable);
}

void RenderCommand::set_blend_func(BlendFactor src_factor,
                                   BlendFactor dest_factor)
{
    rendering_api_->set_blend_func(src_factor, dest_factor);
}

void RenderCommand::set_cull_face(CullFaceType cull_face)
{
    rendering_api_->set_cull_face(cull_face);
}

void RenderCommand::set_front_face(FaceVertexOrder face_vertex_order)
{
    rendering_api_->set_front_face(face_vertex_order);
}

void RenderCommand::set_polygon_mode(PolygonMode polygon_mode)
{
    rendering_api_->set_polygon_mode(polygon_mode);
}

void RenderCommand::set_flip_vertically_on_load(bool flip_vertically)
{
    rendering_api_->set_flip_vertically_on_load(flip_vertically);
}

}  // namespace samui
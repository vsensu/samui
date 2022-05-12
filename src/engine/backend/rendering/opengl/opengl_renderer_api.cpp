// clang-format off
#include "opengl_renderer_api.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <debug/instrumentor.h>
#include "gl_errors.h"
// clang-format on

namespace samui
{
void OpenGLRendererAPI::init()
{
    SAMUI_PROFILE_FUNCTION();

    initGLDebug();

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_DEPTH_TEST);
}

void OpenGLRendererAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                     uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRendererAPI::set_clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRendererAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRendererAPI::draw_indexed(
    const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count)
{
    uint32_t count = index_count == 0
                         ? vertex_array->get_index_buffer()->get_count()
                         : index_count;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRendererAPI::set_depth_test_enable(bool enable)
{
    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
}

void OpenGLRendererAPI::set_depth_test_func(DepthFunc depth_func)
{
    GLenum gl_depth_func = GL_LESS;
    switch (depth_func)
    {
        case DepthFunc::Always:
            gl_depth_func = GL_ALWAYS;
            break;
        case DepthFunc::Never:
            gl_depth_func = GL_NEVER;
            break;
        case DepthFunc::Less:
            gl_depth_func = GL_LESS;
            break;
        case DepthFunc::Equal:
            gl_depth_func = GL_EQUAL;
            break;
        case DepthFunc::Less_Equal:
            gl_depth_func = GL_LEQUAL;
            break;
        case DepthFunc::Greater:
            gl_depth_func = GL_GREATER;
            break;
        case DepthFunc::Not_Equal:
            gl_depth_func = GL_NOTEQUAL;
            break;
        case DepthFunc::Greater_Equal:
            gl_depth_func = GL_GEQUAL;
            break;
    }
    glDepthFunc(gl_depth_func);
}

void OpenGLRendererAPI::set_cull_face_enable(bool enable)
{
    if (enable)
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void OpenGLRendererAPI::set_blend_enable(bool enable)
{
    if (enable)
    {
        glEnable(GL_BLEND);
    }
    else
    {
        glDisable(GL_BLEND);
    }
}

GLenum blend_factor_2_gl(BlendFactor factor)
{
    switch (factor)
    {
        case BlendFactor::Src_Alpha:
            return GL_SRC_ALPHA;
        case BlendFactor::One_Minus_Src_Alpha:
            return GL_ONE_MINUS_SRC_ALPHA;
    }

    return GL_ZERO;
}

void OpenGLRendererAPI::set_blend_func(BlendFactor src_factor,
                                       BlendFactor dest_factor)
{
    glBlendFunc(blend_factor_2_gl(src_factor), blend_factor_2_gl(dest_factor));
}

void OpenGLRendererAPI::set_cull_face(CullFaceType cull_face)
{
    switch (cull_face)
    {
        case CullFaceType::Front:
            glCullFace(GL_FRONT);
            break;
        case CullFaceType::Back:
            glCullFace(GL_BACK);
            break;
        case CullFaceType::FrontAndBack:
            glCullFace(GL_FRONT_AND_BACK);
            break;
    }
}

void OpenGLRendererAPI::set_front_face(FaceVertexOrder face_vertex_order)
{
    switch (face_vertex_order)
    {
        case FaceVertexOrder::Clockwise:
            glFrontFace(GL_CW);
            break;
        case FaceVertexOrder::CounterClockwise:
            glFrontFace(GL_CCW);
            break;
    }
}

void OpenGLRendererAPI::set_polygon_mode(PolygonMode polygon_mode)
{
    switch (polygon_mode)
    {
        case PolygonMode::Wireframe:
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            break;
        case PolygonMode::Fill:
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            break;
    }
}

void OpenGLRendererAPI::set_flip_vertically_on_load(bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
}

}  // namespace samui
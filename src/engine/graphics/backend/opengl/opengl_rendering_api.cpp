#ifdef SAMUI_RENDER_OPENGL

// clang-format off
#include "opengl_rendering_api.h"

#include <glad/glad.h>
#include <stb_image.h>

#include <log/log.h>
#include <assert/assert.h>
#include <profiler/instrumentor.h>

#include "gl_errors.h"
// clang-format on

namespace samui
{
void OpenGLRenderingAPI::init()
{
    SAMUI_PROFILE_FUNCTION();

    initGLDebug();

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glEnable(GL_DEPTH_TEST);
}

void OpenGLRenderingAPI::set_viewport(uint32_t x, uint32_t y, uint32_t width,
                                      uint32_t height)
{
    glViewport(x, y, width, height);
}

void OpenGLRenderingAPI::set_clear_color(const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
}

void OpenGLRenderingAPI::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void OpenGLRenderingAPI::draw_indexed(
    const std::shared_ptr<VertexArray>& vertex_array, uint32_t index_count)
{
    uint32_t count = index_count == 0
                         ? vertex_array->get_index_buffer()->get_count()
                         : index_count;
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    // glBindTexture(GL_TEXTURE_2D, 0);
}

void OpenGLRenderingAPI::set_depth_test_enable(bool enable)
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

void OpenGLRenderingAPI::set_depth_test_func(DepthFunc depth_func)
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

void OpenGLRenderingAPI::set_cull_face_enable(bool enable)
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

void OpenGLRenderingAPI::set_blend_enable(bool enable)
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
        case BlendFactor::Zero:
            return GL_ZERO;
        case BlendFactor::One:
            return GL_ONE;
        case BlendFactor::Src_Color:
            return GL_SRC_COLOR;
        case BlendFactor::One_Minus_Src_Color:
            return GL_ONE_MINUS_SRC_COLOR;
        case BlendFactor::Dst_Color:
            return GL_DST_COLOR;
        case BlendFactor::One_Minus_Dst_Color:
            return GL_ONE_MINUS_DST_COLOR;
        case BlendFactor::Src_Alpha:
            return GL_SRC_ALPHA;
        case BlendFactor::One_Minus_Src_Alpha:
            return GL_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::Dst_Alpha:
            return GL_DST_ALPHA;
        case BlendFactor::One_Minus_Dst_Alpha:
            return GL_ONE_MINUS_DST_ALPHA;
        case BlendFactor::Constant_Color:
            return GL_CONSTANT_COLOR;
        case BlendFactor::One_Minus_Constant_Color:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case BlendFactor::Constant_Alpha:
            return GL_CONSTANT_ALPHA;
        case BlendFactor::One_Minus_Constant_Alpha:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
    }

    return GL_ZERO;
}

void OpenGLRenderingAPI::set_blend_func(BlendFactor src_factor,
                                        BlendFactor dest_factor)
{
    glBlendFunc(blend_factor_2_gl(src_factor), blend_factor_2_gl(dest_factor));
}

void OpenGLRenderingAPI::set_blend_func_separate(BlendFactor src_rgb,
                                                 BlendFactor dst_rgb,
                                                 BlendFactor src_alpha,
                                                 BlendFactor dst_alpha)
{
    glBlendFuncSeparate(blend_factor_2_gl(src_rgb), blend_factor_2_gl(dst_rgb),
                        blend_factor_2_gl(src_alpha),
                        blend_factor_2_gl(dst_alpha));
}

void OpenGLRenderingAPI::set_cull_face(CullFaceType cull_face)
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

void OpenGLRenderingAPI::set_front_face(FaceVertexOrder face_vertex_order)
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

void OpenGLRenderingAPI::set_polygon_mode(PolygonMode polygon_mode)
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

void OpenGLRenderingAPI::set_flip_vertically_on_load(bool flip_vertically)
{
    stbi_set_flip_vertically_on_load(flip_vertically ? 1 : 0);
}

}  // namespace samui

#endif
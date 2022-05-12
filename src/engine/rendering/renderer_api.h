#pragma once

// clang-format off
#include <memory>

#include <glm/glm.hpp>

#include "buffer.h"
#include "core/core.h"
// clang-format on

namespace samui
{
enum class SAMUI_API CullFaceType
{
    Front,
    Back,
    FrontAndBack
};

enum class SAMUI_API FaceVertexOrder
{
    Clockwise,
    CounterClockwise
};

enum class SAMUI_API PolygonMode
{
    Wireframe,
    Fill
};

enum class SAMUI_API BlendFactor
{
    Src_Alpha,
    One_Minus_Src_Alpha,
};

enum class SAMUI_API DepthFunc
{
    Always,
    Never,
    Less,
    Equal,
    Less_Equal,
    Greater,
    Not_Equal,
    Greater_Equal
};

class SAMUI_API RendererAPI
{
  public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

  public:
    virtual ~RendererAPI() {}

    virtual void init() = 0;
    virtual void set_viewport(uint32_t x, uint32_t y, uint32_t width,
                              uint32_t height) = 0;
    virtual void set_clear_color(const glm::vec4& color) = 0;
    virtual void clear() = 0;
    virtual void draw_indexed(const std::shared_ptr<VertexArray>& vertex_array,
                              uint32_t index_count = 0) = 0;
    virtual void set_depth_test_enable(bool enable) = 0;
    virtual void set_depth_test_func(DepthFunc depth_func) = 0;
    virtual void set_cull_face_enable(bool enable) = 0;
    virtual void set_blend_enable(bool enable) = 0;
    virtual void set_blend_func(BlendFactor src_factor,
                                BlendFactor dest_factor) = 0;
    virtual void set_cull_face(CullFaceType cull_face) = 0;
    virtual void set_front_face(FaceVertexOrder face_vertex_order) = 0;
    virtual void set_polygon_mode(PolygonMode polygon_mode) = 0;
    virtual void set_flip_vertically_on_load(bool flip_vertically) = 0;

    inline static API get_api() { return api_; }

  private:
    static API api_;
};
}  // namespace samui

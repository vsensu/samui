// clang-format off
#include "render_command.h"

#include <backend/rendering/opengl/opengl_renderer_api.h>
// clang-format on

namespace samui {
RendererAPI* RenderCommand::renderer_api_ = new OpenGLRendererAPI();

}  // namespace samui
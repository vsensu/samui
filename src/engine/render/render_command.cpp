#include "render_command.h"

#include "opengl_renderer_api.h"

namespace samui {
RendererAPI* RenderCommand::renderer_api_ = new OpenGLRendererAPI();

}  // namespace samui
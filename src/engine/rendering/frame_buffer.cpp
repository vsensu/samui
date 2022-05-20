// // clang-format off
// #include "frame_buffer.h"

// #include "renderer.h"
// #ifdef SAMUI_RENDER_OPENGL
// #include <backend/rendering/opengl/opengl_frame_buffer.h>
// #endif
// // clang-format on

// namespace samui
// {
// std::shared_ptr<FrameBuffer> FrameBuffer::create(
//     const FrameBufferSpecification& spec)
// {
//     switch (Renderer::get_api())
//     {
//         case RendererAPI::API::OpenGL:
//         {
// #ifdef SAMUI_RENDER_OPENGL
//             return std::make_shared<OpenGLFrameBuffer>(spec);
// #endif
//         }
//     }
//     SAMUI_ENGINE_ASSERT(false, "Unsupported Render API");
//     return nullptr;
// }
// }  // namespace samui

#include "renderer.h"

namespace samui {
void Renderer::BeginScene() {}

void Renderer::EndScene() {}

void Renderer::Submit(const std::shared_ptr<VertexArray>& va) {
    va->Bind();
    RenderCommand::DrawIndexed(va);
}
}  // namespace samui

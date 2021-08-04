// clang-format off
#include "scene.h"

#include <glm/glm.hpp>

#include "components.h"
#include <rendering/renderer2d.h>
// clang-format on

namespace samui {

Scene::Scene() {
}

entt::entity Scene::CreateEntity() { return registry_.create(); }

void Scene::OnUpdate(const Timestep& deltaTime) {
  auto view = registry_.view<TransformComponent, SpriteRendererComponent>();

  view.each([](const auto& transform, const auto& sprite) {
    Renderer2D::DrawQuad(transform.transform, sprite.color);
  });
}

}  // namespace samui

// clang-format off
#include "scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components.h"
#include <rendering/renderer2d.h>
// clang-format on

namespace samui {

Scene::Scene() {}

Entity Scene::CreateEntity() {
  auto entity = registry_.create();
  AddComponent<TransformComponent>(entity, glm::identity<glm::mat4>());
  return entity;
}

void Scene::OnUpdate(const Timestep& deltaTime) {
  auto view = registry_.view<TransformComponent, SpriteRendererComponent>();

  view.each([](const auto& transform, const auto& sprite) {
    Renderer2D::DrawQuad(transform.transform, sprite.color);
  });
}

}  // namespace samui

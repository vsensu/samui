// clang-format off
#include "scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "components.h"
#include <rendering/renderer2d.h>
// clang-format on

namespace samui {

Scene::Scene() {}

Entity Scene::CreateEntity(const std::string& name) {
  auto entity = registry_.create();
  AddComponent<NameComponent>(entity, name);
  AddComponent<TransformComponent>(entity);
  return entity;
}

void Scene::DestroyEntity(Entity entity) { registry_.destroy(entity); }

void Scene::OnUpdate(const Timestep& deltaTime) {}

}  // namespace samui

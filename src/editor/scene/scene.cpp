// clang-format off
#include "scene.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/graphics/renderer/renderer2d.h>

#include "components.h"
// clang-format on

namespace samui
{

Scene::Scene() {}

Entity Scene::create_entity(const std::string& name)
{
    auto entity = registry_.create();
    add_component<NameComponent>(entity, name);
    add_component<TransformComponent>(entity);
    return entity;
}

void Scene::destroy_entity(Entity entity) { registry_.destroy(entity); }

void Scene::on_update(const Timestep& deltaTime) {}

}  // namespace samui

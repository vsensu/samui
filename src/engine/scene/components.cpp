#include "components.h"

#include <entt/entt.hpp>

namespace samui {
void foo_components() {
  entt::registry registry;
  registry.view<NameComponent>();
  registry.view<TransformComponent>();
  registry.view<SpriteRendererComponent>();
  registry.view<CameraComponent>();
}
}  // namespace samui

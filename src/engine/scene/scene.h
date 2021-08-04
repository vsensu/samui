#ifndef SAMUI_SCENE_H_
#define SAMUI_SCENE_H_

// clang-format off
#include <entt/entt.hpp>

#include <core/core.h>
#include <core/timestep.h>
#include <log/log.h>
// clang-format on

namespace samui {
using Entity = entt::entity;
class SAMUI_API Scene {
 public:
  Scene();

  Entity CreateEntity();

  template <typename T, typename... Args>
  T& AddComponent(Entity entity, Args&&... args) {
    SAMUI_ENGINE_ASSERT(!HasComponent<T>(entity), "entity already has component!");
    return registry_.emplace<T>(entity, std::forward<Args>(args)...);
  }

  template <typename T>
  T& GetComponent(Entity entity) {
    SAMUI_ENGINE_ASSERT(HasComponent<T>(entity), "entity does not has component!");
    return registry_.get<T>(entity);
  }

  template <typename T>
  bool HasComponent(Entity entity) {
    return registry_.any_of<T>(entity);
  }

  template <typename T>
  void RemoveComponent(Entity entity) {
    SAMUI_ENGINE_ASSERT(HasComponent<T>(entity), "entity does not has component!");
    registry_.remove<T>(entity);
  }

  entt::registry& registry() { return registry_; }

  void OnUpdate(const Timestep& deltaTime);

 private:
  entt::registry registry_;
};

}  // namespace samui

#endif
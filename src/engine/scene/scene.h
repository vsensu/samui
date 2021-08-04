#ifndef SAMUI_SCENE_H_
#define SAMUI_SCENE_H_

// clang-format off
#include <core/core.h>
#include <core/timestep.h>

#include <entt/entt.hpp>
// clang-format on

namespace samui {
class SAMUI_API Scene {
 public:
  Scene();

  entt::entity CreateEntity();

  entt::registry& registry() { return registry_; }

  void OnUpdate(const Timestep& deltaTime);

 private:
  entt::registry registry_;
};

}  // namespace samui

#endif
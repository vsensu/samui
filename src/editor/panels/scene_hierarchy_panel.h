#pragma once

// clang-format off
#include <memory>

// #include <samui.h>
#include "../scene/scene.h"
// clan-format on

namespace samui {
class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

  void OnImGuiRender();

  void SetScene(std::shared_ptr<Scene> scene);

  Entity GetSelectedEntity() const { return selected_entity_; }
  void SetSelectedEntity(Entity entity) { selected_entity_ = entity; }

 private:
  void DrawProperties(Entity entity);

 private:
  std::shared_ptr<Scene> scene_;
  Entity     selected_entity_{entt::null};
};
}  // namespace samui

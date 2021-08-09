#ifndef SAMUI_SCENE_HIERARCHY_PANEL_H_
#define SAMUI_SCENE_HIERARCHY_PANEL_H_

#include <samui.h>

namespace samui {
class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const Ref<Scene>& scene);

  void OnImGuiRender();

  void SetScene(Ref<Scene> scene);

  Entity GetSelectedEntity() const { return selected_entity_; }
  void SetSelectedEntity(Entity entity) { selected_entity_ = entity; }

 private:
  void DrawProperties(Entity entity);

 private:
  Ref<Scene> scene_;
  Entity     selected_entity_{entt::null};
};
}  // namespace samui

#endif  // SAMUI_SCENE_HIERARCHY_PANEL_H_
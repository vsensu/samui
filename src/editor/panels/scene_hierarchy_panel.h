#ifndef SAMUI_SCENE_HIERARCHY_PANEL_H_
#define SAMUI_SCENE_HIERARCHY_PANEL_H_

#include <samui.h>

namespace samui {
class SceneHierarchyPanel {
 public:
  SceneHierarchyPanel() = default;
  SceneHierarchyPanel(const Ref<Scene>& scene);

  void OnImGuiRender();

 private:
  Ref<Scene> scene_;
  Entity     selected_entity_{entt::null};
};
}  // namespace samui

#endif  // SAMUI_SCENE_HIERARCHY_PANEL_H_
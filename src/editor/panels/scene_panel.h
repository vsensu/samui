#ifndef SAMUI_SCENE_PANEL_H_
#define SAMUI_SCENE_PANEL_H_

// clang-format off
#include <samui.h>

#include <editor_camera.h>
// clang-format on

namespace samui {

class EditorLayer;
class ScenePanel {
 public:
  ScenePanel(const Ref<Scene>& scene, EditorLayer* parent);

  void SetScene(const Ref<Scene>& scene);

  void OnUpdate(const Timestep& deltaTime);
  void RenderScene();
  void OnImGuiRender();
  void OnEvent(Event& event);

  Entity              GetHoveredEntity() const { return hovered_entity_; }
  ImGuizmo::OPERATION GetGizmosOp() const { return gizmos_op_; }
  void                SetGizmosOp(ImGuizmo::OPERATION op) { gizmos_op_ = op; }

 private:
  Ref<Scene>   scene_;
  EditorLayer* parent_;

  glm::vec2           viewport_bounds_[2];
  bool                viewport_focused_{false};
  bool                viewport_hovered_{false};
  Ref<FrameBuffer>    frame_buffer_;
  Entity              hovered_entity_{entt::null};
  EditorCamera        editor_camera_;
  ImGuizmo::OPERATION gizmos_op_{ImGuizmo::TRANSLATE};
};
}  // namespace samui

#endif  // SAMUI_SCENE_PANEL_H_
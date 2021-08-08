#ifndef SAMUI_EDITOR_LAYER_H_
#define SAMUI_EDITOR_LAYER_H_

#include <samui.h>

#include "panels/scene_hierarchy_panel.h"

namespace samui {
class EditorLayer : public Layer {
 public:
  EditorLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(const Timestep& deltaTime) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Event& event) override;

 private:
  void OnImGuiFullScreenDocking();

  void NewScene();
  void LoadScene();
  void SaveSceneAs();

  bool OnKeyPressed(KeyPressedEvent& event);

 private:
  OrthographicCameraController camera_controller_;

  Ref<FrameBuffer> frame_buffer_;
  Ref<Scene>       active_scene_;
  ImVec2           last_viewport_size_;
  bool             viewport_focused_{false};
  bool             viewport_hovered_{false};
  Entity           square_entity_;
  Entity           main_camera_{entt::null};
  Entity           first_camera_{entt::null};
  Entity           second_camera_{entt::null};

  Ref<SceneHierarchyPanel> scene_hierarchy_panel_;

  ImGuizmo::OPERATION gizmos_op_{ImGuizmo::TRANSLATE};
};

}  // namespace samui

#endif  // SAMUI_EDITOR_LAYER_H_
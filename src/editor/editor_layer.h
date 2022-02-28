#ifndef SAMUI_EDITOR_LAYER_H_
#define SAMUI_EDITOR_LAYER_H_

// clang-format off
#include <memory>
#include <filesystem>

#include <samui.h>

#include <panels/scene_hierarchy_panel.h>
#include <panels/content_browser.h>
#include <panels/scene_panel.h>
// #include "editor_camera.h"
// clang-format on

namespace samui {
class EditorLayer : public Layer {
 public:
  EditorLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnUpdate(const Timestep& deltaTime) override;
  virtual void OnImGuiRender() override;
  virtual void OnEvent(Event& event) override;

  void NewScene();
  void OpenScene();
  void OpenScene(const std::filesystem::path& path);
  void SaveSceneAs();

  Entity GetSelectedEntity();

 private:
  void OnImGuiFullScreenDocking();

  bool OnKeyPressed(KeyPressedEvent& event);
  bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

 private:
  OrthographicCameraController camera_controller_;

  std::shared_ptr<Scene> active_scene_;

  Entity square_entity_;
  Entity main_camera_{entt::null};
  Entity first_camera_{entt::null};
  Entity second_camera_{entt::null};

  std::shared_ptr<SceneHierarchyPanel> scene_hierarchy_panel_;
  std::shared_ptr<ContentBrowser>      content_browser_;
  std::shared_ptr<ScenePanel>          scene_panel_;
};

}  // namespace samui

#endif  // SAMUI_EDITOR_LAYER_H_
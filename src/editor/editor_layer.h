#ifndef SAMUI_EDITOR_LAYER_H_
#define SAMUI_EDITOR_LAYER_H_

#include <samui.h>

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
};

}  // namespace samui

#endif  // SAMUI_EDITOR_LAYER_H_
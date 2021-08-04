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
  entt::entity     square_entity_;
};

}  // namespace samui

#endif  // SAMUI_EDITOR_LAYER_H_
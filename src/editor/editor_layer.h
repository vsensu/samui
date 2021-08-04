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

  glm::vec4        square_color_{glm::vec4(1.f)};
  Ref<FrameBuffer> frame_buffer_;
  ImVec2           last_viewport_size_;
  bool             viewport_focused_{false};
  bool             viewport_hovered_{false};
};

}  // namespace samui

#endif  // SAMUI_EDITOR_LAYER_H_
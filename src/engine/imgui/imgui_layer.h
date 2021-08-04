#ifndef SAMUI_IMGUI_LAYER_H_
#define SAMUI_IMGUI_LAYER_H_

// clang-format off
#include <core/core.h>
#include <events/event.h>
#include <layer/layer.h>
// clang-format on

namespace samui {
class SAMUI_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  virtual ~ImGuiLayer();

  void         OnAttach() override;
  void         OnDetach() override;
  virtual void OnEvent(Event& event) override;

  void Begin();
  void End();

  void BlockEvents(bool block) { block_events_ = block; }

 private:
  bool block_events_{true};
};

}  // namespace samui

#endif
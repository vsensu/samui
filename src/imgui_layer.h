#ifndef SAMUI_IMGUI_LAYER_H_
#define SAMUI_IMGUI_LAYER_H_

#include "core.h"
#include "events/event.h"
#include "layer.h"

namespace samui {
class SAMUI_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  virtual ~ImGuiLayer();

  void OnAttach() override;
  void OnDetach() override;
  void OnUpdate() override;
  void OnEvent(Event& event) override;
};

}  // namespace samui

#endif
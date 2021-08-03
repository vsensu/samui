#ifndef SAMUI_IMGUI_LAYER_H_
#define SAMUI_IMGUI_LAYER_H_

#include "../core/core.h"
#include "../events/event.h"
#include "../layer/layer.h"

namespace samui {
class SAMUI_API ImGuiLayer : public Layer {
 public:
  ImGuiLayer();
  virtual ~ImGuiLayer();

  void OnAttach() override;
  void OnDetach() override;

  void Begin();
  void End();
};

}  // namespace samui

#endif
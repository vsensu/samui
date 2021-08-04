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

  void OnAttach() override;
  void OnDetach() override;

  void Begin();
  void End();
};

}  // namespace samui

#endif
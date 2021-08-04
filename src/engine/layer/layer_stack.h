#ifndef SAMUI_LAYER_STACK_H_
#define SAMUI_LAYER_STACK_H_

// clang-format off
#include <core/core.h>
#include "layer.h"
// clang-format on

namespace samui {
class SAMUI_API LayerStack {
 public:
  LayerStack();
  ~LayerStack();

  void PushLayer(Layer* layer);
  void PushOverlay(Layer* overlay);
  void PopLayer(Layer* layer);
  void PopOverlay(Layer* overlay);

  std::vector<Layer*>::iterator begin() { return layers_.begin(); }
  std::vector<Layer*>::iterator end() { return layers_.end(); }

 private:
  std::vector<Layer*>           layers_;
  unsigned int layer_insert_index_ {0};
};

}  // namespace samui

#endif
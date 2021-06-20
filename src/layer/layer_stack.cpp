#include "layer_stack.h"

namespace samui {
LayerStack::LayerStack() { layer_insert_ = layers_.begin(); }

LayerStack::~LayerStack() {
  for (Layer* layer : layers_) {
    delete layer;
  }
}

void LayerStack::PushLayer(Layer* layer) {
  layer_insert_ = layers_.emplace(layer_insert_, layer);
}

void LayerStack::PushOverlay(Layer* overlay) { layers_.emplace_back(overlay); }

void LayerStack::PopLayer(Layer* layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);
  if (it != layers_.end()) {
    layers_.erase(it);
    --layer_insert_;
  }
}

void LayerStack::PopOverlay(Layer* layer) {
  auto it = std::find(layers_.begin(), layers_.end(), layer);
  if (it != layers_.end()) {
    layers_.erase(it);
  }
}

}  // namespace samui

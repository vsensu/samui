#pragma once

// clang-format off
#include <vector>

#include "_inc.h"
#include "layer.h"
// clang-format on

namespace samui
{
class SAMUI_CORE_API LayerStack
{
  public:
    LayerStack();
    ~LayerStack();

    void push_layer(Layer* layer);
    void push_overlay(Layer* overlay);
    void pop_layer(Layer* layer);
    void pop_overlay(Layer* overlay);

    std::vector<Layer*>::iterator begin() { return layers_.begin(); }
    std::vector<Layer*>::iterator end() { return layers_.end(); }

  private:
    std::vector<Layer*> layers_;
    unsigned int        layer_insert_index_{0};
};

}  // namespace samui

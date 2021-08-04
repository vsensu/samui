#ifndef SAMUI_LAYER_H_
#define SAMUI_LAYER_H_

// clang-format off
#include <core/core.h>
#include <core/timestep.h>
#include <events/event.h>
// clang-format on

namespace samui {

class SAMUI_API Layer {
 public:
  Layer(const std::string& debugName = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate(const Timestep &deltaTime) {}
  virtual void OnImGuiRender() {}
  virtual void OnEvent(Event& event) {}

  inline const std::string& GetName() const { return debug_name_; }

 protected:
  std::string debug_name_;
};

}  // namespace samui

#endif
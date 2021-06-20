#ifndef SAMUI_LAYER_H_
#define SAMUI_LAYER_H_

#include "../core/core.h"
#include "../events/event.h"

namespace samui {

class SAMUI_API Layer {
 public:
  Layer(const std::string& debugName = "Layer");
  virtual ~Layer();

  virtual void OnAttach() {}
  virtual void OnDetach() {}
  virtual void OnUpdate() {}
  virtual void OnEvent(Event& event) {}

  inline const std::string& GetName() const { return debug_name_; }

 protected:
  std::string debug_name_;
};

}  // namespace samui

#endif
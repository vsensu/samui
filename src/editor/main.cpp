// clang-format off
#include <samui.h>
#include <entrypoint.h>

#include <imgui.h>

#include "editor_layer.h"
// clang-format on

namespace samui {

class Demo : public Application {
 public:
  Demo() : Application() { PushLayer(new EditorLayer()); }
};

Application* create_application() { return new Demo(); }

}  // namespace samui
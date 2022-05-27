// clang-format off
// #include <samui.h>
#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>

#include "editor_layer.h"
// clang-format on

namespace samui
{

class EditorApplication : public GraphicsApplication
{
  public:
    EditorApplication() : GraphicsApplication() { push_layer(new EditorLayer()); }
};

std::shared_ptr<Application> create_application()
{
    return std::make_shared<EditorApplication>();
}

}  // namespace samui
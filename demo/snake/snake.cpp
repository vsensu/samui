// clang-format off
#include <samui.h>
#include <entrypoint.h>

#include "sprite_atlas.h"
#include "snake.h"
// clang-format on

class Demo : public samui::Application
{
  public:
    Demo() : Application(samui::WindowProps("Snake", 1280, 720, false)) { push_layer(new Game2DLayer()); }
};

samui::Application* samui::create_application()
{
    SAMUI_ENGINE_TRACE("engine trace");
    SAMUI_ENGINE_INFO("engine info");
    SAMUI_ENGINE_WARN("engine warn");
    SAMUI_ENGINE_ERROR("engine error");
    SAMUI_ENGINE_FATAL("engine fatal");

    SAMUI_TRACE("game trace");
    SAMUI_INFO("game info");
    SAMUI_WARN("game warn");
    SAMUI_ERROR("game error");
    SAMUI_FATAL("game fatal");

    return new Demo();
}
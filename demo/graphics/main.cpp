// clang-format off
#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>
// clang-format on

class GameLayer : public samui::Layer
{
    void on_attach() override { SAMUI_INFO("game layer attach"); }
};

class Demo : public samui::GraphicsApplication
{
  public:
    Demo() : GraphicsApplication() { push_layer(new GameLayer()); }
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

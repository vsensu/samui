// clang-format off
#include <engine/log/log.h>
#include <engine/console/console_application.h>
#include <engine/entrypoint.h>
// clang-format on

class GameLayer : public samui::Layer
{
    void on_attach() override { SAMUI_INFO("game layer attach"); }
    virtual void on_update(const samui::Timestep& deltaTime) override
    {
        SAMUI_INFO("game layer update");
    }
};

class Demo : public samui::ConsoleApplication
{
  public:
    Demo() : ConsoleApplication() { push_layer(new GameLayer()); }
};

std::shared_ptr<samui::Application> samui::create_application()
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

    return std::make_shared<Demo>();
}
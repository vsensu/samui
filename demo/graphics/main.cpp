// clang-format off
#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <imgui/imgui.h>
// clang-format on

class GameLayer : public samui::Layer
{
  public:
    virtual void on_attach() override { SAMUI_INFO("game layer attach"); }
    virtual void on_imgui_render() override
    {
        auto stats = samui::Renderer2D::get_stats();
        ImGui::Begin("Stats");
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.draw_calls);
        ImGui::Text("Quads: %d", stats.quad_count);
        ImGui::End();
    }
};

class Demo : public samui::GraphicsApplication
{
  public:
    Demo() : GraphicsApplication() { push_layer(new GameLayer()); }
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

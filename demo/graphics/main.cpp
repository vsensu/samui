// clang-format off
#include <engine/log/log.h>
#include <engine/graphics/graphics_application.h>
#include <engine/entrypoint.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/renderer/render_command.h>
#include <engine/graphics/renderer/texture_create.h>
#include <imgui/imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
// clang-format on

class GameLayer : public samui::Layer
{
  public:
    GameLayer()
    {
        texture_ = samui::texture2d::create("assets/textures/Checkerboard.png");
    }
    virtual void on_attach() override
    {
        SAMUI_INFO("game layer attach");
        samui::RenderCommand::set_flip_vertically_on_load(true);
        samui::RenderCommand::set_cull_face_enable(true);
        samui::RenderCommand::set_blend_func(
            samui::BlendFactor::Src_Alpha,
            samui::BlendFactor::One_Minus_Src_Alpha);
        samui::RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
    }

    virtual void on_update(const samui::Timestep& deltaTime) override
    {
        samui::RenderCommand::clear();
        samui::Renderer2D::reset_stats();

        glm::mat4 projection = glm::ortho(0.f, 1280.f, -720.f, 0.f, -1.f, 1.f);
        samui::Renderer2D::begin_scene(projection);

        samui::RenderCommand::set_depth_test_enable(true);
        samui::RenderCommand::set_blend_enable(false);
        samui::Renderer2D::draw_quad({32.f, -32.f}, {64.f, 64.f},
                                     {1.0f, 0.0f, 0.0f, 1.0f});
        samui::Renderer2D::draw_quad({200.f, -200.f}, {128.f, 128.f}, texture_);

        samui::RenderCommand::set_depth_test_enable(false);
        samui::RenderCommand::set_blend_enable(true);
        samui::Renderer2D::draw_quad({48.f, -48.f}, {64.f, 64.f},
                                     {0.0f, 1.0f, 0.0f, 0.3f});

        samui::Renderer2D::end_scene();
    }

    virtual void on_imgui_render() override
    {
        auto stats = samui::Renderer2D::get_stats();
        ImGui::Begin("Stats");
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.draw_calls);
        ImGui::Text("Quads: %d", stats.quad_count);
        ImGui::End();

        ImGui::ShowDemoWindow();
    }

  private:
    std::shared_ptr<samui::Texture2D> texture_;
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

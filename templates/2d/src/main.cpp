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
#include <glm/gtc/type_ptr.hpp>
// clang-format on

class Game2DLayer : public samui::Layer
{
  public:
    Game2DLayer() : Layer("2D Game")
    {
        samui::RenderCommand::set_flip_vertically_on_load(true);
        samui::RenderCommand::set_cull_face_enable(true);
        samui::RenderCommand::set_blend_func(
            samui::BlendFactor::Src_Alpha,
            samui::BlendFactor::One_Minus_Src_Alpha);
        samui::RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
    }

    virtual void on_attach() override { SAMUI_PROFILE_FUNCTION(); }

    virtual void on_detach() override { SAMUI_PROFILE_FUNCTION(); }

    virtual void on_update(const samui::Timestep& deltaTime)
    {
        SAMUI_PROFILE_FUNCTION();

        samui::Renderer2D::reset_stats();
        {
            SAMUI_PROFILE_SCOPE("Render Prepare");
        }

        {
            SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
            samui::RenderCommand::clear();
            samui::Renderer2D::reset_stats();

            glm::mat4 projection =
                glm::ortho(0.f, 1280.f, -720.f, 0.f, -1.f, 1.f);
            samui::Renderer2D::begin_scene(projection);

            samui::RenderCommand::set_depth_test_enable(true);
            samui::RenderCommand::set_blend_enable(false);
            samui::Renderer2D::draw_quad({32.f, -32.f}, {64.f, 64.f},
                                         {1.0f, 0.0f, 0.0f, 1.0f});

            samui::RenderCommand::set_depth_test_enable(false);
            samui::RenderCommand::set_blend_enable(true);
            samui::Renderer2D::draw_quad({48, -48.f}, {64.f, 64.f},
                                         square_color_);
            samui::Renderer2D::end_scene();
        }
    }

    virtual void on_imgui_render()
    {
        auto stats = samui::Renderer2D::get_stats();
        ImGui::Begin("Settings");
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.draw_calls);
        ImGui::Text("Quads: %d", stats.quad_count);
        ImGui::ColorEdit4("square color", glm::value_ptr(square_color_));
        ImGui::End();
    }

  private:
    glm::vec4 square_color_{glm::vec4(1.f)};
};

class Demo : public samui::GraphicsApplication
{
  public:
    Demo() : GraphicsApplication() { push_layer(new Game2DLayer()); }
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

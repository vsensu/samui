// clang-format off
#include <iostream>
#include <chrono>

#include <samui.h>
#include <entrypoint.h>
#include <glm/gtc/type_ptr.hpp>
// clang-format on

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {}

  virtual void OnAttach() override {
    SAMUI_PROFILE_FUNCTION();
    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    camera_controller_.SetZoomLevel(5.f);
  }

  virtual void OnDetach() override { SAMUI_PROFILE_FUNCTION(); }

  virtual void OnUpdate(const samui::Timestep& deltaTime) {
    SAMUI_PROFILE_FUNCTION();
    camera_controller_.OnUpdate(deltaTime);

    samui::Renderer2D::ResetStats();
    {
      SAMUI_PROFILE_SCOPE("Render Prepare");
      samui::RenderCommand::Clear();
    }

    {
      SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
      samui::RenderCommand::Clear();
      samui::Renderer2D::BeginScene(camera_controller_.GetCamera());
      samui::Renderer2D::DrawQuad({0, 0}, {0.5f, 0.5f}, square_color_);
      samui::Renderer2D::EndScene();
    }
  }

  virtual void OnImGuiRender() {
    auto stats = samui::Renderer2D::GetStats();
    ImGui::Begin("Settings");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::ColorEdit4("square color", glm::value_ptr(square_color_));
    ImGui::End();
  }

  virtual void OnEvent(samui::Event& event) {
    camera_controller_.OnEvent(event);
  }

 private:
  samui::OrthographicCameraController camera_controller_;
  glm::vec4 square_color_{glm::vec4(1.f)};
};

class Demo : public samui::Application {
 public:
  Demo() : Application() { push_layer(new Game2DLayer()); }
};

samui::Application* samui::create_application() {
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

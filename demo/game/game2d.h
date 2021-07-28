#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

#include <imgui.h>
#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {}

  virtual void OnAttach() override {
    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  }
  virtual void OnDetach() override {}
  virtual void OnUpdate(const samui::Timestep& deltaTime) {
    camera_controller_.OnUpdate(deltaTime);

    samui::RenderCommand::Clear();
    samui::Renderer2D::BeginScene(camera_controller_.GetCamera());
    samui::Renderer2D::DrawQuad({0.f, 0.f}, {1.f, 1.f}, square_color_);
    samui::Renderer2D::EndScene();
  }
  virtual void OnImGuiRender() {
    ImGui::Begin("Settings");
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

#endif  // GAME_GAME2D_H_
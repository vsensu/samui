#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

#include <imgui.h>
#include <samui.h>

#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {}

  virtual void OnAttach() override {
    SAMUI_PROFILE_FUNCTION();
    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    texture_ = samui::Texture2D::Create("assets/textures/Checkerboard.png");
  }

  virtual void OnDetach() override { SAMUI_PROFILE_FUNCTION(); }

  virtual void OnUpdate(const samui::Timestep& deltaTime) {
    SAMUI_PROFILE_FUNCTION();
    camera_controller_.OnUpdate(deltaTime);

    {
      SAMUI_PROFILE_SCOPE("Render Prepare");
      samui::RenderCommand::Clear();
    }

    {
      SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
      samui::Renderer2D::BeginScene(camera_controller_.GetCamera());
      // samui::Renderer2D::DrawRotatedQuad({-1.f, 0.f}, {0.8f, 0.8f},
      //  glm::radians(45.f), square_color_);
      samui::Renderer2D::DrawQuad({-1.f, 2.f}, {0.5f, 0.5f}, square_color_);
      samui::Renderer2D::DrawQuad({-1.f, 1.f, 0.f}, {0.2f, 0.5f},
                                  {0.2f, 0.8f, 0.4f, 1.f});
      samui::Renderer2D::DrawQuad({-1.f, 0.f}, {0.5f, 0.5f}, texture_, 1.f,
                                  glm::vec4(1.f, 0.8f, 0.8f, 1.f));
      samui::Renderer2D::DrawQuad({-1.f, -1.f, 0.f}, {0.5f, 0.5f}, texture_,
                                  10.f, glm::vec4(1.f, 0.5f, 0.5f, 1.f));
      samui::Renderer2D::DrawRotatedQuad({1.f, 2.f}, {0.5f, 0.5f},
                                         glm::radians(45.f), square_color_);
      samui::Renderer2D::DrawRotatedQuad({1.f, 1.f, 0.f}, {0.5f, 0.5f},
                                         glm::radians(45.f), square_color_);
      samui::Renderer2D::DrawRotatedQuad({1.f, 0.f}, {0.5f, 0.5f},
                                         glm::radians(45.f), texture_, 10.f,
                                         glm::vec4(1.f, 0.8f, 0.8f, 1.f));
      samui::Renderer2D::DrawRotatedQuad({1.f, -1.f, 0.f}, {0.5f, 0.5f},
                                         glm::radians(45.f), texture_, 10.f,
                                         glm::vec4(1.f, 0.8f, 0.8f, 1.f));
      samui::Renderer2D::EndScene();
    }
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

  glm::vec4                    square_color_{glm::vec4(1.f)};
  samui::Ref<samui::Texture2D> texture_;
};

#endif  // GAME_GAME2D_H_
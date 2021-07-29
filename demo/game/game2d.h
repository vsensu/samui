#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

#include <imgui.h>
#include <samui.h>

#include <chrono>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

template <typename TCallBack>
class Timer {
 public:
  Timer(const char* name, const TCallBack& callback)
      : name_(name), callback_(callback), stopped_(false) {
    start_point_ = std::chrono::high_resolution_clock::now();
  }

  void Stop() {
    auto end_point = std::chrono::high_resolution_clock::now();

    long long start =
        std::chrono::time_point_cast<std::chrono::microseconds>(start_point_)
            .time_since_epoch()
            .count();
    long long end =
        std::chrono::time_point_cast<std::chrono::microseconds>(end_point)
            .time_since_epoch()
            .count();

    stopped_ = true;

    callback_({name_, end - start});
  }

  ~Timer() {
    if (!stopped_) {
      Stop();
    }
  }

 private:
  const char*                                        name_;
  TCallBack                                          callback_;
  std::chrono::time_point<std::chrono::steady_clock> start_point_;
  bool                                               stopped_;
};

#define PROFILE_SCOPE(name)                                      \
  Timer timer##__LINE__(name, [&](const ProfileResult& result) { \
    profiles_.push_back(result);                                 \
  });

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {}

  virtual void OnAttach() override {
    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    texture_ = samui::Texture2D::Create("assets/textures/Checkerboard.png");
  }
  virtual void OnDetach() override {}
  virtual void OnUpdate(const samui::Timestep& deltaTime) {
    PROFILE_SCOPE("Game2D::OnUpdate");
    {
      PROFILE_SCOPE("CameraController::OnUpdate");
      camera_controller_.OnUpdate(deltaTime);
    }

    {
      PROFILE_SCOPE("Render Prepare");
      samui::RenderCommand::Clear();
    }

    {
      PROFILE_SCOPE("Render Draw(CPU)");
      samui::Renderer2D::BeginScene(camera_controller_.GetCamera());
      samui::Renderer2D::DrawQuad({-1.f, 0.f}, {0.8f, 0.8f}, square_color_);
      samui::Renderer2D::DrawQuad({0.5f, 0.5f}, {0.2f, 0.5f},
                                  {0.2f, 0.8f, 0.4f, 1.f});
      samui::Renderer2D::DrawQuad({0.f, 0.f, -0.1f}, {10.f, 10.f}, texture_);
      samui::Renderer2D::EndScene();
    }
  }
  virtual void OnImGuiRender() {
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("square color", glm::value_ptr(square_color_));

    for (const auto& profile : profiles_) {
      char buf[128];
      strcpy(buf, "%.3fms ");
      strcat(buf, profile.name);
      ImGui::Text(buf, profile.microseconds * 0.001f);
    }

    ImGui::End();

    profiles_.clear();
  }

  virtual void OnEvent(samui::Event& event) {
    camera_controller_.OnEvent(event);
  }

 private:
  samui::OrthographicCameraController camera_controller_;

  glm::vec4                    square_color_{glm::vec4(1.f)};
  samui::Ref<samui::Texture2D> texture_;

  struct ProfileResult {
    const char* name;
    long long   microseconds;
  };

  std::vector<ProfileResult> profiles_;
};

#endif  // GAME_GAME2D_H_
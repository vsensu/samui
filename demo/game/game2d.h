#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

// clang-format off
#include <iostream>
#include <chrono>

#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
// clang-format on

class TileLevel {
 public:
  TileLevel(const char* levelString, const glm::uvec2& mapSize)
      : level_str_(levelString), map_size_(mapSize) {}

  void Draw() {
    for (uint32_t y = 0; y < map_size_.y; ++y) {
      for (uint32_t x = 0; x < map_size_.x; ++x) {
        char tile_type = level_str_[y * map_size_.x + x];
        samui::Ref<samui::SubTexture2D> texture = tiles_[tile_type];
        samui::Renderer2D::DrawQuad(
            {x - map_size_.x / 2.f, y - map_size_.y / 2.f, 0.5f}, {1.f, 1.f},
            texture);
      }
    }
  }

  void SetTileDefines(
      const std::unordered_map<char, samui::Ref<samui::SubTexture2D>>&
          tileDefines) {
    tiles_ = tileDefines;
  }

 private:
  const char*                                               level_str_;
  glm::uvec2                                                map_size_;
  std::unordered_map<char, samui::Ref<samui::SubTexture2D>> tiles_;
  // samui::Ref<samui::Texture2D>                              sprite_sheet_;
};

const char* level_str =
    "WWWW"
    "WDDW"
    "WWWW";

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {}

  virtual void OnAttach() override {
    SAMUI_PROFILE_FUNCTION();
    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
    sprite_sheet_ =
        samui::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

    tile_level_ = std::make_shared<TileLevel>(level_str, glm::uvec2(4, 3));
    std::unordered_map<char, samui::Ref<samui::SubTexture2D>> tileDefines = {
        {'D', samui::SubTexture2D::CreateFromCoords(sprite_sheet_, {6, 11},
                                                    {128, 128})},
        {'W', samui::SubTexture2D::CreateFromCoords(sprite_sheet_, {11, 11},
                                                    {128, 128})},
    };
    tile_level_->SetTileDefines(tileDefines);

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
      tile_level_->Draw();
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

  glm::vec4                      square_color_{glm::vec4(1.f)};
  samui::Ref<samui::Texture2D>   sprite_sheet_;
  samui::Ref<TileLevel>          tile_level_;
};

#endif  // GAME_GAME2D_H_
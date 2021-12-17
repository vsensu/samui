#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

// clang-format off
#include <iostream>
#include <chrono>

#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
// clang-format on

class TileLevel
{
  public:
    TileLevel(const char* levelString, const glm::uvec2& mapSize)
        : level_str_(levelString), map_size_(mapSize)
    {
    }

    void Draw()
    {
        constexpr float tile_size = 32.f;
        for (int y = 0; y < map_size_.y; ++y)
        {
            for (int x = 0; x < map_size_.x; ++x)
            {
                char tile_type = level_str_[y * map_size_.x + x];
                samui::Ref<samui::SubTexture2D> texture = tiles_[tile_type];
                auto screen_pos = screen_pos_to_world_pos(
                    {x * tile_size, y * tile_size-720});
                samui::Renderer2D::DrawQuad({screen_pos.x, screen_pos.y, 0.f},
                                            {tile_size, tile_size}, texture);
            }
        }
    }

    void SetTileDefines(
        const std::unordered_map<char, samui::Ref<samui::SubTexture2D>>&
            tileDefines)
    {
        tiles_ = tileDefines;
    }

  private:
    glm::vec2 screen_pos_to_world_pos(const glm::vec2& screen_pos)
    {
        // screen space
        // negative x to positive x from left to right
        // negative y to positive y from top to bottom
        // opengel world space
        // negative x to positive x from left to right
        // negative y to positive y from bottom to top
        // so we need to flip y
        return {screen_pos.x, -screen_pos.y};
        // return {screen_pos.x, screen_pos.y};
    }

  private:
    const char*                                               level_str_;
    glm::uvec2                                                map_size_;
    std::unordered_map<char, samui::Ref<samui::SubTexture2D>> tiles_;
};

const char* level_str =
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WDDDW" "WWDWW" "WWDWWWDWW" "WDWWDW" "WDDDW"
    "WDWWW" "WDWDW" "WDWDWDWDW" "WDWWDW" "WWDWW"
    "WDDDW" "WDDDW" "WDWWDWWDW" "WDWWDW" "WWDWW"
    "WWWDW" "WDWDW" "WDWWWWWDW" "WDWWDW" "WWDWW"
    "WDDDW" "WDWDW" "WDWWWWWDW" "WWDDWW" "WDDDW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW";

class Game2DLayer : public samui::Layer
{
  public:
    Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true)
    {
    }

    virtual void OnAttach() override
    {
        SAMUI_PROFILE_FUNCTION();
        samui::RenderCommand::SetDepthTestEnable(false);
        samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        sprite_sheet_ =
            samui::Texture2D::Create("assets/textures/RPGpack_sheet_2X.png");

        tile_level_ = std::make_shared<TileLevel>(level_str, glm::uvec2(30, 7));
        std::unordered_map<char, samui::Ref<samui::SubTexture2D>> tileDefines =
            {
                {'D', samui::SubTexture2D::CreateFromCoords(
                          sprite_sheet_, {6, 11}, {128, 128})},
                {'W', samui::SubTexture2D::CreateFromCoords(
                          sprite_sheet_, {11, 11}, {128, 128})},
            };
        tile_level_->SetTileDefines(tileDefines);

        camera_controller_.SetZoomLevel(10.f);
    }

    virtual void OnDetach() override { SAMUI_PROFILE_FUNCTION(); }

    virtual void OnUpdate(const samui::Timestep& deltaTime)
    {
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
            // glm::mat4 projection = glm::ortho(0.f, 1280.f, 720.f, 0.f, -1.f, 1.f);
            glm::mat4 projection = glm::ortho(0.f, 1280.f, 0.f, 720.f, -1.f, 1.f);
            // samui::Renderer2D::BeginScene(camera_controller_.GetCamera());
            samui::Renderer2D::BeginScene(/*glm::translate(glm::identity<glm::mat4>(), {1280*0.5f, -720*0.5f, 0}) **/ projection);
            tile_level_->Draw();
            samui::Renderer2D::EndScene();
        }
    }

    virtual void OnImGuiRender()
    {
        auto stats = samui::Renderer2D::GetStats();
        ImGui::Begin("Stats");
        ImGui::Text("Renderer2D Stats:");
        ImGui::Text("Draw Calls: %d", stats.draw_calls);
        ImGui::Text("Quads: %d", stats.quad_count);
        ImGui::End();
    }

    virtual void OnEvent(samui::Event& event)
    {
        camera_controller_.OnEvent(event);
    }

  private:
    samui::OrthographicCameraController camera_controller_;

    samui::Ref<samui::Texture2D> sprite_sheet_;
    samui::Ref<TileLevel>        tile_level_;
};

#endif  // GAME_GAME2D_H_
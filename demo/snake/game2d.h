#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

// clang-format off
#include <iostream>
#include <array>
#include <list>

#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
// clang-format on

constexpr float                         tile_size = 128.f;
constexpr int                           rows = 10;
constexpr int                           cols = 6;
std::array<std::array<int, cols>, rows> level;

struct SnakeNode
{
  public:
    int row, col;
};

enum class Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class TileLevel
{
  public:
    TileLevel(const char* levelString, const glm::uvec2& mapSize)
        : level_str_(levelString), map_size_(mapSize)
    {
    }

    void Draw()
    {
        for (int y = 0; y < map_size_.y; ++y)
        {
            for (int x = 0; x < map_size_.x; ++x)
            {
                char tile_type = level_str_[y * map_size_.x + x];
                samui::Ref<samui::SubTexture2D> texture = tiles_[tile_type];
                auto screen_pos = glm::vec2{x * tile_size, y * tile_size};
                samui::Renderer2D::DrawQuad(
                    {screen_pos.x, screen_pos.y + tile_size * 0.5f, 0.f},
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
    const char*                                               level_str_;
    glm::uvec2                                                map_size_;
    std::unordered_map<char, samui::Ref<samui::SubTexture2D>> tiles_;
};

// clang-format off
const char* level_str =
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW"
    "WWWWW" "WWWWW" "WWWWWWWWW" "WWWWWW" "WWWWW";
// clang-format on

class Game2DLayer : public samui::Layer
{
  public:
    Game2DLayer() : Layer("Example") {}

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

        snake_sprite_ = samui::SubTexture2D::CreateFromCoords(
            sprite_sheet_, {0, 9}, {128, 128});
        snake_.push_back(std::make_shared<SnakeNode>(0, 5));
    }

    virtual void OnDetach() override { SAMUI_PROFILE_FUNCTION(); }

    virtual void OnUpdate(const samui::Timestep& deltaTime)
    {
        SAMUI_PROFILE_FUNCTION();

        timer_ += deltaTime;
        if (timer_ >= move_interval_)
        {
            timer_ = 0.f;
            int new_col = snake_.front()->col;
            int new_row = snake_.front()->row;
            switch (move_direction_)
            {
                case Direction::RIGHT:
                    ++new_col;
                    break;
                case Direction::LEFT:
                    --new_col;
                    break;
                case Direction::UP:
                    --new_row;
                    break;
                case Direction::DOWN:
                    ++new_row;
                    break;
            }
            snake_.pop_back();
            snake_.push_front(std::make_shared<SnakeNode>(new_row, new_col));
        }

        samui::Renderer2D::ResetStats();
        {
            SAMUI_PROFILE_SCOPE("Render Prepare");
            samui::RenderCommand::Clear();
        }

        {
            SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
            samui::RenderCommand::Clear();
            glm::mat4 projection =
                glm::ortho(0.f, 1280.f, 720.f, 0.f, -1.f, 1.f);
            samui::Renderer2D::BeginScene(projection);
            tile_level_->Draw();
            for (auto& snake : snake_)
            {
                samui::Renderer2D::DrawQuad(
                    {snake->col * tile_size,
                     snake->row * tile_size + tile_size * 0.5f, 0.f},
                    {tile_size, tile_size}, snake_sprite_);
            }
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
        if (event.GetEventType() == samui::EventType::KeyReleased)
        {
            auto keyReleasedEvent = static_cast<samui::KeyPressedEvent&>(event);
            auto keyCode = keyReleasedEvent.GetKeyCode();
            if (keyCode == SAMUI_KEY_F2)
            {
                static bool wireframe = false;
                wireframe = !wireframe;
                samui::RenderCommand::SetPolygonMode(
                    wireframe ? samui::PolygonMode::Wireframe
                              : samui::PolygonMode::Fill);
            }

            else if (keyCode == SAMUI_KEY_UP)
            {
                move_direction_ = Direction::UP;
            }
            else if (keyCode == SAMUI_KEY_DOWN)
            {
                move_direction_ = Direction::DOWN;
            }
            else if (keyCode == SAMUI_KEY_LEFT)
            {
                move_direction_ = Direction::LEFT;
            }
            else if (keyCode == SAMUI_KEY_RIGHT)
            {
                move_direction_ = Direction::RIGHT;
            }
        }
    }

  private:
    samui::Ref<samui::Texture2D>          sprite_sheet_;
    samui::Ref<TileLevel>                 tile_level_;
    samui::Ref<samui::SubTexture2D>       snake_sprite_;
    std::list<std::shared_ptr<SnakeNode>> snake_;
    Direction                             move_direction_{Direction::RIGHT};
    float                                 timer_ = 0.f;
    float                                 move_interval_ = 1.f;
};

#endif  // GAME_GAME2D_H_
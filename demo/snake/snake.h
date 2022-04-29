#pragma once

// clang-format off
#include <iostream>
#include <array>
#include <list>
#include <random>
#include <memory>

#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
// clang-format on

constexpr float tile_size = 32.f;
constexpr int   rows = 6;
constexpr int   cols = 6;

enum class GameState
{
    Running,
    Win,
    Lose,
};

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

enum class SnakeSpriteType
{
    NONE,
    HEAD,
    BODY,
    GRASS,
    GRASS_TOP_LEFT_CORNOR,
    GRASS_TOP_EDGE,
    GRASS_TOP_RIGHT_CORNOR,
    GRASS_BOTTOM_LEFT_CORNOR,
    GRASS_BOTTOM_EDGE,
    GRASS_BOTTOM_RIGHT_CORNOR,
    GRASS_LEFT_EDGE,
    GRASS_RIGHT_EDGE,
    FOOD,
    WIN,
    LOSE,
};

std::array<std::array<SnakeSpriteType, cols>, rows> level = {
    SnakeSpriteType::GRASS_TOP_LEFT_CORNOR,
    SnakeSpriteType::GRASS_TOP_EDGE,
    SnakeSpriteType::GRASS_TOP_EDGE,
    SnakeSpriteType::GRASS_TOP_EDGE,
    SnakeSpriteType::GRASS_TOP_EDGE,
    SnakeSpriteType::GRASS_TOP_RIGHT_CORNOR,

    SnakeSpriteType::GRASS_LEFT_EDGE,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS_RIGHT_EDGE,

    SnakeSpriteType::GRASS_LEFT_EDGE,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS_RIGHT_EDGE,

    SnakeSpriteType::GRASS_LEFT_EDGE,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS_RIGHT_EDGE,

    SnakeSpriteType::GRASS_LEFT_EDGE,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS,
    SnakeSpriteType::GRASS_RIGHT_EDGE,

    SnakeSpriteType::GRASS_BOTTOM_LEFT_CORNOR,
    SnakeSpriteType::GRASS_BOTTOM_EDGE,
    SnakeSpriteType::GRASS_BOTTOM_EDGE,
    SnakeSpriteType::GRASS_BOTTOM_EDGE,
    SnakeSpriteType::GRASS_BOTTOM_EDGE,
    SnakeSpriteType::GRASS_BOTTOM_RIGHT_CORNOR};

decltype(level) food_level;

class Game2DLayer : public samui::Layer
{
  public:
    Game2DLayer() : Layer("Example") {}

    virtual void OnAttach() override
    {
        SAMUI_PROFILE_FUNCTION();
        // samui::RenderCommand::SetDepthTestEnable(false);
        samui::RenderCommand::SetCullFaceEnable(true);
        samui::RenderCommand::SetBlendEnable(true);
        samui::RenderCommand::SetBlendFunc(
            samui::BlendFactor::Src_Alpha,
            samui::BlendFactor::One_Minus_Src_Alpha);
        samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});

        sprite_atlas_ = std::make_shared<SpriteAtlas<SnakeSpriteType>>(
            "assets/textures/snake.png");

        sprite_atlas_->add_sprite(SnakeSpriteType::HEAD, {15, 12}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::BODY, {16, 12}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS, {1, 11}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_TOP_LEFT_CORNOR,
                                  {0, 12}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_TOP_EDGE, {1, 12},
                                  {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_TOP_RIGHT_CORNOR,
                                  {2, 12}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_BOTTOM_LEFT_CORNOR,
                                  {0, 10}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_BOTTOM_EDGE, {1, 10},
                                  {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_BOTTOM_RIGHT_CORNOR,
                                  {2, 10}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_LEFT_EDGE, {0, 11},
                                  {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::GRASS_RIGHT_EDGE, {2, 11},
                                  {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::FOOD, {8, 0}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::WIN, {17, 12}, {128, 128});
        sprite_atlas_->add_sprite(SnakeSpriteType::LOSE, {18, 12}, {128, 128});

        snake_head_sprite_ = sprite_atlas_->get_sprite(SnakeSpriteType::HEAD);
        snake_body_sprite_ = sprite_atlas_->get_sprite(SnakeSpriteType::BODY);

        snake_.push_back(std::make_shared<SnakeNode>(0, 5));
        gen_food();
    }

    virtual void OnDetach() override { SAMUI_PROFILE_FUNCTION(); }

    virtual void OnUpdate(const samui::Timestep& deltaTime)
    {
        SAMUI_PROFILE_FUNCTION();

        if (game_state_ == GameState::Running)
        {
            move_interval_ = samui::Input::IsKeyPressed(SAMUI_KEY_LEFT_SHIFT)
                                 ? fast_move_interval_
                                 : norm_move_interval_;

            timer_ += deltaTime;
            if (timer_ >= move_interval_)
            {
                timer_ = 0.f;
                int new_col = snake_.front()->col;
                int new_row = snake_.front()->row;
                move_direction_ = next_move_direction_;
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
                if (new_col >= cols)
                {
                    new_col = 0;
                }
                if (new_col < 0)
                {
                    new_col = cols - 1;
                }
                if (new_row >= rows)
                {
                    new_row = 0;
                }
                if (new_row < 0)
                {
                    new_row = rows - 1;
                }

                auto tail = snake_.back();
                snake_.pop_back();

                // death check
                bool dead = false;
                for (const auto& snake : snake_)
                {
                    if (snake->row == new_row && snake->col == new_col)
                    {
                        dead = true;
                        break;
                    }
                }
                if (dead)
                {
                    // TODO: Game Over
                    game_state_ = GameState::Lose;
                }

                // grow check
                if (food_level[new_row][new_col] == SnakeSpriteType::NONE)
                {
                    snake_.push_front(
                        std::make_shared<SnakeNode>(new_row, new_col));
                }
                else
                {
                    // eat food
                    food_level[new_row][new_col] = SnakeSpriteType::NONE;

                    // grow up
                    snake_.push_front(
                        std::make_shared<SnakeNode>(new_row, new_col));
                    // add tail
                    snake_.push_back(tail);

                    // gen new food
                    if (!gen_food())
                    {
                        game_state_ = GameState::Win;
                    }
                }
            }
        }

        samui::Renderer2D::ResetStats();
        {
            SAMUI_PROFILE_SCOPE("Render Prepare");
            samui::RenderCommand::Clear();
        }

        {
            SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
            samui::RenderCommand::Clear();
            const auto& window = samui::Application::instance().get_window();
            auto        window_width = static_cast<float>(window.GetWidth());
            auto        window_height = static_cast<float>(window.GetHeight());
            glm::mat4   projection =
                glm::ortho(0.f, window_width, 0.f, window_height, -1.f, 1.f);
            samui::Renderer2D::BeginScene(projection);
            for (int row = 0; row < rows; ++row)
            {
                for (int col = 0; col < cols; ++col)
                {
                    // // draw background
                    // samui::Renderer2D::DrawQuad(
                    //     {col * tile_size + tile_size,
                    //      row * tile_size + tile_size, 0.f},
                    //     {tile_size, -tile_size},
                    //     sprite_atlas_->get_sprite(SnakeSpriteType::GRASS));

                    // draw battle ground
                    samui::Renderer2D::DrawQuad(
                        {col * tile_size + tile_size,
                         window_height - row * tile_size - tile_size, 0.f},
                        {tile_size, tile_size},
                        sprite_atlas_->get_sprite(level[row][col]));

                    // draw food
                    if (food_level[row][col] != SnakeSpriteType::NONE)
                    {
                        samui::Renderer2D::DrawQuad(
                            {col * tile_size + tile_size,
                             window_height - row * tile_size - tile_size, 0.f},
                            {tile_size, tile_size},
                            sprite_atlas_->get_sprite(food_level[row][col]));
                    }
                }
            }
            int i = 0;
            for (auto& snake : snake_)
            {
                auto sprite = i == 0 ? snake_head_sprite_ : snake_body_sprite_;
                samui::Renderer2D::DrawQuad(
                    {snake->col * tile_size + tile_size,
                     window_height - snake->row * tile_size - tile_size, 0.f},
                    {tile_size, tile_size}, sprite);
                ++i;
            }

            if (game_state_ == GameState::Win)
            {
                samui::Renderer2D::DrawQuad(
                    {tile_size, window_height - tile_size, 0.f},
                    {tile_size * cols, tile_size * rows},
                    sprite_atlas_->get_sprite(SnakeSpriteType::WIN));
            }
            else if (game_state_ == GameState::Lose)
            {
                samui::Renderer2D::DrawQuad(
                    {tile_size, window_height - tile_size, 0.f},
                    {tile_size * cols, tile_size * rows},
                    sprite_atlas_->get_sprite(SnakeSpriteType::LOSE));
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
                if (move_direction_ == Direction::UP)
                {
                    force_move();
                }

                if (move_direction_ != Direction::DOWN)
                {
                    next_move_direction_ = Direction::UP;
                    force_move();
                }
            }
            else if (keyCode == SAMUI_KEY_DOWN)
            {
                if (move_direction_ == Direction::DOWN)
                {
                    force_move();
                }

                if (move_direction_ != Direction::UP)
                {
                    next_move_direction_ = Direction::DOWN;
                    force_move();
                }
            }
            else if (keyCode == SAMUI_KEY_LEFT)
            {
                if (move_direction_ == Direction::LEFT)
                {
                    force_move();
                }

                if (move_direction_ != Direction::RIGHT)
                {
                    next_move_direction_ = Direction::LEFT;
                    force_move();
                }
            }
            else if (keyCode == SAMUI_KEY_RIGHT)
            {
                if (move_direction_ == Direction::RIGHT)
                {
                    force_move();
                }

                if (move_direction_ != Direction::LEFT)
                {
                    next_move_direction_ = Direction::RIGHT;
                    force_move();
                }
            }

            else if (keyCode == SAMUI_KEY_G)
            {
                auto head = snake_.front();
                int  new_row = head->row;
                int  new_col = head->col + 1;
                snake_.push_front(
                    std::make_shared<SnakeNode>(new_row, new_col));
            }

            else if (keyCode == SAMUI_KEY_F)
            {
                gen_food();
            }
        }
    }

  private:
    bool gen_food()
    {
        std::vector<std::pair<int, int>> pool;
        for (int row = 0; row < rows; ++row)
        {
            for (int col = 0; col < cols; ++col)
            {
                bool occupy = false;
                if (food_level[row][col] != SnakeSpriteType::NONE)
                {
                    occupy = true;
                }
                else
                {
                    for (const auto& snake : snake_)
                    {
                        if (snake->row == row && snake->col == col)
                        {
                            occupy == true;
                            break;
                        }
                    }
                }

                if (!occupy)
                {
                    pool.push_back({row, col});
                }
            }
        }

        if (pool.size() > 0)
        {
            std::uniform_int_distribution<> uniform_dist(0, pool.size() - 1);
            auto                            random_index = uniform_dist(mt);
            auto [row, col] = pool[random_index];
            food_level[row][col] = SnakeSpriteType::FOOD;

            return true;
        }

        return false;
    }

    void force_move() { timer_ += move_interval_ + 1.f; }

  private:
    std::shared_ptr<SpriteAtlas<SnakeSpriteType>> sprite_atlas_;
    std::shared_ptr<samui::SubTexture2D>               snake_head_sprite_;
    std::shared_ptr<samui::SubTexture2D>               snake_body_sprite_;
    std::list<std::shared_ptr<SnakeNode>>         snake_;
    Direction          move_direction_{Direction::RIGHT};
    Direction          next_move_direction_{move_direction_};
    float              timer_ = 0.f;
    float              fast_move_interval_ = 0.2f;
    float              norm_move_interval_ = 1.f;
    float              move_interval_{norm_move_interval_};
    std::random_device rd{};
    std::mt19937       mt{rd()};
    GameState          game_state_{GameState::Running};
};

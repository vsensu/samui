#ifndef GAME_GAME2D_H_
#define GAME_GAME2D_H_

#include <imgui.h>
#include <samui.h>

#include <glm/gtc/type_ptr.hpp>
#include <iostream>

class Game2DLayer : public samui::Layer {
 public:
  Game2DLayer() : Layer("Example"), camera_controller_(1280.f / 720.f, true) {
    shader_library_ = std::make_shared<samui::ShaderLibrary>();

    square_vertex_array_.reset(samui::VertexArray::Create());
    // clang-format off
    float square_vertices[3 * 4] = {
      -0.5f, -0.5f, 0.f,
      0.5f, -0.5f, 0.f,
      0.5f, 0.5f, 0.f,
      -0.5f, 0.5f, 0.f
    };
    // clang-format on
    samui::Ref<samui::VertexBuffer> square_vertex_buffer;
    square_vertex_buffer.reset(
        samui::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
    samui::BufferLayout square_layout = {
        {"Position", samui::ShaderDataType::Float3},
    };
    square_vertex_buffer->SetLayout(square_layout);
    square_vertex_array_->AddVertexBuffer(square_vertex_buffer);
    uint32_t                       square_indices[6] = {0, 1, 2, 2, 3, 0};
    samui::Ref<samui::IndexBuffer> square_index_buffer;
    square_index_buffer.reset(samui::IndexBuffer::Create(
        square_indices, sizeof(square_indices) / sizeof(uint32_t)));
    square_vertex_array_->SetIndexBuffer(square_index_buffer);

    shader_ = samui::Shader::Create("assets/shaders/flatcolor.glsl");
    shader_library_->Add("FlatColor", shader_);
  }

  virtual void OnAttach() override {}
  virtual void OnDetach() override {}
  virtual void OnUpdate(const samui::Timestep& deltaTime) {
    camera_controller_.OnUpdate(deltaTime);

    samui::RenderCommand::Clear();
    samui::Renderer::BeginScene(camera_controller_.GetCamera());
    shader_->Bind();
    shader_->UploadUniform("u_color", square_color_);
    samui::Renderer::Submit(shader_, square_vertex_array_);
    samui::Renderer::EndScene();
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
  samui::Ref<samui::ShaderLibrary>    shader_library_;
  samui::OrthographicCameraController camera_controller_;
  samui::Ref<samui::Shader>           shader_;
  samui::Ref<samui::VertexArray>      square_vertex_array_;

  glm::vec4 square_color_{glm::vec4(1.f)};
};

#endif  // GAME_GAME2D_H_
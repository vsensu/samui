#include <imgui.h>
#include <samui.h>

#include <iostream>

class ExampleLayer : public samui::Layer {
 public:
  ExampleLayer() : Layer("Example"), camera_(-1.6f, 1.6f, -0.9f, 0.9f) {
    vertex_array_.reset(samui::VertexArray::Create());

    // clang-format off
    float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
      0.5f, -0.5f, 0.f, 0.2f, 0.3f,  0.8f, 1.f,
      0.f,  0.5f, 0.f, 0.8f, 0.8f, 0.2f,  1.f,
    };
    // clang-format on
    std::shared_ptr<samui::VertexBuffer> vertex_buffer;
    vertex_buffer.reset(
        samui::VertexBuffer::Create(vertices, sizeof(vertices)));
    samui::BufferLayout layout = {
        {"Position", samui::ShaderDataType::Float3},
        {"Color", samui::ShaderDataType::Float4},
    };
    vertex_buffer->SetLayout(layout);
    vertex_array_->AddVertexBuffer(vertex_buffer);
    uint32_t                            indices[3] = {0, 1, 2};
    std::shared_ptr<samui::IndexBuffer> index_buffer;
    index_buffer.reset(samui::IndexBuffer::Create(
        indices, sizeof(indices) / sizeof(uint32_t)));
    vertex_array_->SetIndexBuffer(index_buffer);

    square_vertex_array_.reset(samui::VertexArray::Create());
    // clang-format off
    float square_vertices[3 * 4] = {
      -0.5f, -0.5f, 0.f,
      0.5f, -0.5f, 0.f,
      0.5f, 0.5f, 0.f,
      -0.5f, 0.5f, 0.f,
    };
    // clang-format on
    std::shared_ptr<samui::VertexBuffer> square_vertex_buffer;
    square_vertex_buffer.reset(
        samui::VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
    samui::BufferLayout square_layout = {
        {"Position", samui::ShaderDataType::Float3},
    };
    square_vertex_buffer->SetLayout(square_layout);
    square_vertex_array_->AddVertexBuffer(square_vertex_buffer);
    uint32_t                            square_indices[6] = {0, 1, 2, 2, 3, 0};
    std::shared_ptr<samui::IndexBuffer> square_index_buffer;
    square_index_buffer.reset(samui::IndexBuffer::Create(
        square_indices, sizeof(square_indices) / sizeof(uint32_t)));
    square_vertex_array_->SetIndexBuffer(square_index_buffer);

    const std::string g_vs_code = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 viewProj;
uniform mat4 transform;

out vec4 vertexColor;

void main()
{
    gl_Position = viewProj * transform * vec4(aPos, 1.0);
    vertexColor = aColor;
}
)";

    const std::string g_fs_code = R"(
#version 330 core
out vec4 FragColor;

in vec4 vertexColor;

void main()
{
  FragColor = vertexColor;
}
)";

    shader_.reset(samui::Shader::Create(g_vs_code.c_str(), g_fs_code.c_str()));

    const std::string g_vs_code_blue = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewProj;
uniform mat4 transform;

void main()
{
    gl_Position = viewProj * transform * vec4(aPos, 1.0);
}
)";

    const std::string g_fs_code_blue = R"(
#version 330 core
out vec4 FragColor;

void main()
{
  FragColor = vec4(0.2, 0.3, 0.8, 1.0);
}
)";
    blue_shader_.reset(
        samui::Shader::Create(g_vs_code_blue.c_str(), g_fs_code_blue.c_str()));

    samui::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  }
  void OnUpdate(const samui::Timestep& deltaTime) override {
    if (samui::Input::IsKeyPressed(SAMUI_KEY_TAB)) {
      SAMUI_TRACE("Tab is pressed!");
    }
    SAMUI_INFO("delta time: {0}s, {1}ms", deltaTime.time_in_seconds(),
               deltaTime.time_in_milliseconds());

    auto camera_pos = camera_.get_position();
    if (samui::Input::IsKeyPressed(SAMUI_KEY_LEFT)) {
      camera_pos.x -= 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_RIGHT)) {
      camera_pos.x += 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_UP)) {
      camera_pos.y += 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_DOWN)) {
      camera_pos.y -= 1.f * deltaTime;
    }

    camera_.set_position(camera_pos);

    if (samui::Input::IsKeyPressed(SAMUI_KEY_J)) {
      square_pos_.x -= 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_L)) {
      square_pos_.x += 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_I)) {
      square_pos_.y += 1.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_K)) {
      square_pos_.y -= 1.f * deltaTime;
    }

    auto rotation = camera_.get_rotation();
    if (samui::Input::IsKeyPressed(SAMUI_KEY_A)) {
      rotation += 10.f * deltaTime;
    } else if (samui::Input::IsKeyPressed(SAMUI_KEY_D)) {
      rotation -= 10.f * deltaTime;
    }
    camera_.set_rotation(rotation);

    samui::RenderCommand::Clear();

    samui::Renderer::BeginScene(camera_);

    static glm::mat4 scale =
        glm::scale(glm::identity<glm::mat4>(), glm::vec3(0.1f));

    for (int y = 0; y < 20; ++y) {
      for (int x = 0; x < 20; ++x) {
        glm::vec3 pos = square_pos_ + glm::vec3(x * 0.11f, y * 0.11f, 0.f);
        auto      transform =
            glm::translate(glm::identity<glm::mat4>(), pos) * scale;
        samui::Renderer::Submit(blue_shader_, square_vertex_array_, transform);
      }
    }

    samui::Renderer::Submit(shader_, vertex_array_);
    samui::Renderer::EndScene();
  }

  void OnEvent(samui::Event& event) override {
    if (event.GetEventType() == samui::EventType::KeyPressed) {
      auto& key_press_event = static_cast<samui::KeyPressedEvent&>(event);
      SAMUI_TRACE("Input:{0}", (char)key_press_event.GetKeyCode());
    }

    samui::EventDispatcher dispatcher(event);
    dispatcher.Dispatch<samui::KeyPressedEvent>(
        BIND_EVENT_FUNC(ExampleLayer::OnKeyPressedEvent));
  }

  void OnImGuiRender() override {
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
  }

  bool OnKeyPressedEvent(samui::KeyPressedEvent& event) { return false; }

 private:
  std::shared_ptr<samui::Shader>      shader_;
  std::shared_ptr<samui::VertexArray> vertex_array_;

  std::shared_ptr<samui::Shader>      blue_shader_;
  std::shared_ptr<samui::VertexArray> square_vertex_array_;

  samui::OrthographicCamera camera_;

  glm::vec3 square_pos_{glm::zero<glm::vec3>()};
};

class Demo : public samui::Application {
 public:
  Demo() : Application() { PushLayer(new ExampleLayer()); }
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

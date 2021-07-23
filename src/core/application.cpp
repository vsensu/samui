#include "application.h"

#include "../imgui/imgui_layer.h"
#include "../log/log.h"
#include "../render/buffer.h"
#include "../render/renderer.h"

namespace samui {
#define BIND_EVENT_FUNC(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application(/* args */) : camera_(-1.0f, 1.0f, -1.0f, 1.0f) {
  instance_ = this;
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

  imgui_layer_ = new ImGuiLayer();
  PushOverlay(imgui_layer_);

  vertex_array_.reset(VertexArray::Create());
  // clang-format off
  float vertices[3 * 7] = {
      -0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
      0.5f, -0.5f, 0.f, 0.2f, 0.3f,  0.8f, 1.f,
      0.f,  0.5f, 0.f, 0.8f, 0.8f, 0.2f,  1.f,
  };
  // clang-format on
  std::shared_ptr<VertexBuffer> vertex_buffer;
  vertex_buffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
  BufferLayout layout = {
      {"Position", ShaderDataType::Float3},
      {"Color", ShaderDataType::Float4},
  };
  vertex_buffer->SetLayout(layout);
  vertex_array_->AddVertexBuffer(vertex_buffer);
  uint32_t                     indices[3] = {0, 1, 2};
  std::shared_ptr<IndexBuffer> index_buffer;
  index_buffer.reset(
      IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
  vertex_array_->SetIndexBuffer(index_buffer);

  square_vertex_array_.reset(VertexArray::Create());
  // clang-format off
  float square_vertices[3 * 4] = {
      -0.75f, -0.75f, 0.f,
      0.75f, -0.75f, 0.f,
      0.75f, 0.75f, 0.f,
      -0.75f, 0.75f, 0.f,
  };
  // clang-format on
  std::shared_ptr<VertexBuffer> square_vertex_buffer;
  square_vertex_buffer.reset(
      VertexBuffer::Create(square_vertices, sizeof(square_vertices)));
  BufferLayout square_layout = {
      {"Position", ShaderDataType::Float3},
  };
  square_vertex_buffer->SetLayout(square_layout);
  square_vertex_array_->AddVertexBuffer(square_vertex_buffer);
  uint32_t                     square_indices[6] = {0, 1, 2, 2, 3, 0};
  std::shared_ptr<IndexBuffer> square_index_buffer;
  square_index_buffer.reset(IndexBuffer::Create(
      square_indices, sizeof(square_indices) / sizeof(uint32_t)));
  square_vertex_array_->SetIndexBuffer(square_index_buffer);

  const std::string g_vs_code = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

uniform mat4 viewProj;

out vec4 vertexColor;

void main()
{
    gl_Position = viewProj * vec4(aPos, 1.0);
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

  shader_.reset(
      new Shader<CreateShaderProgramFromString>(g_vs_code, g_fs_code));

  const std::string g_vs_code_blue = R"(
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 viewProj;

void main()
{
    gl_Position = viewProj * vec4(aPos, 1.0);
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
  blue_shader_.reset(new Shader<CreateShaderProgramFromString>(g_vs_code_blue,
                                                               g_fs_code_blue));

  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
}

Application::~Application() {}

void Application::Run() {
  while (running_) {
    RenderCommand::Clear();
    Renderer::BeginScene(camera_);

    window_->BeforeUpdate();

    blue_shader_->Use();
    blue_shader_->LoadUniform("viewProj", camera_.get_view_projection_matrix());
    Renderer::Submit(square_vertex_array_);

    shader_->Use();
    shader_->LoadUniform("viewProj", camera_.get_view_projection_matrix());
    Renderer::Submit(vertex_array_);

    Renderer::EndScene();

    for (Layer* layer : layer_stack_) {
      layer->OnUpdate();
    }

    imgui_layer_->Begin();
    for (Layer* layer : layer_stack_) {
      layer->OnImGuiRender();
    }
    imgui_layer_->End();

    window_->OnUpdate();
    window_->LateUpdate();
  }
}

void Application::PushLayer(Layer* layer) {
  layer_stack_.PushLayer(layer);
  layer->OnAttach();
}

void Application::PushOverlay(Layer* layer) {
  layer_stack_.PushOverlay(layer);
  layer->OnAttach();
}

void Application::OnEvent(Event& e) {
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNC(OnWindowClose));

  for (auto it = layer_stack_.end(); it != layer_stack_.begin();) {
    (*--it)->OnEvent(e);
    if (e.handled_) {
      break;
    }
  }
}

bool Application::OnWindowClose(WindowCloseEvent& event) {
  running_ = false;
  return true;
}

}  // namespace samui
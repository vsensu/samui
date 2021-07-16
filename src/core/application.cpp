#include "application.h"

#include <glad/glad.h>

#include "../imgui/imgui_layer.h"
#include "../log/log.h"
#include "../render/buffer.h"

namespace samui {
#define BIND_EVENT_FUNC(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

Application::Application(/* args */) {
  instance_ = this;
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

  imgui_layer_ = new ImGuiLayer();
  PushOverlay(imgui_layer_);

  vertex_array_.reset(VertexArray::Create());

  // 顶点数据
  float vertices[3*7] = {
    -0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
  0.5f, -0.5f, 0.f, 0.2f, 0.3f, 0.8f, 1.f,
  0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f,
  };

  vertex_buffer_.reset(VertexBuffer::Create(vertices, sizeof(vertices)));

  BufferLayout layout = {
      {"Position", ShaderDataType::Float3},
      {"Color", ShaderDataType::Float4},
  };

  vertex_buffer_->SetLayout(layout);
  vertex_array_->AddVertexBuffer(vertex_buffer_);

  uint32_t indices[3] = {0, 1, 2};
  index_buffer_.reset(IndexBuffer::Create(indices, sizeof(indices)/sizeof(uint32_t)));
  vertex_array_->SetIndexBuffer(index_buffer_);

  const std::string g_vs_code = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;

out vec4 vertexColor;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
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

  // 顶点着色器
  GLuint        vertexShader = glCreateShader(GL_VERTEX_SHADER);
  const GLchar* vsCode = g_vs_code.c_str();
  glShaderSource(vertexShader, 1, &vsCode, nullptr);
  glCompileShader(vertexShader);
  int  success;
  char infoLog[512];
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
    SAMUI_ENGINE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
    SAMUI_ENGINE_ERROR(infoLog);
  }

  // 片段着色器
  GLuint        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  const GLchar* fsCode = g_fs_code.c_str();
  glShaderSource(fragmentShader, 1, &fsCode, nullptr);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    SAMUI_ENGINE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
    SAMUI_ENGINE_ERROR(infoLog);
  }

  // Shader program
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    SAMUI_ENGINE_ERROR("ERROR::SHADER::LINK_FAILED");
    SAMUI_ENGINE_ERROR(infoLog);
  }
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glClearColor(0, 0, 0, 1);
}

Application::~Application() {}

void Application::Run() {
  while (running_) {
    glClear(GL_COLOR_BUFFER_BIT);

    window_->BeforeUpdate();

    // 当我们渲染一个物体时要使用着色器程序
    glUseProgram(shaderProgram);

    vertex_array_->Bind();

    // 绘制物体
    glDrawElements(GL_TRIANGLES, index_buffer_->GetCount(), GL_UNSIGNED_INT, nullptr);

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
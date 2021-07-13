#include "application.h"

#include <glad/glad.h>

#include "../imgui/imgui_layer.h"
#include "../log/log.h"
#include "../render/buffer.h"

namespace samui {
#define BIND_EVENT_FUNC(x) \
  std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::instance_ = nullptr;

// clang-format off
static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
  switch (type) {
    case ShaderDataType::Float:   return GL_FLOAT;
    case ShaderDataType::Float2:  return GL_FLOAT;
    case ShaderDataType::Float3:  return GL_FLOAT;
    case ShaderDataType::Float4:  return GL_FLOAT;
    case ShaderDataType::Mat3:    return GL_FLOAT;
    case ShaderDataType::Mat4:    return GL_FLOAT;
    case ShaderDataType::Int:     return GL_INT;
    case ShaderDataType::Int2:    return GL_INT;
    case ShaderDataType::Int3:    return GL_INT;
    case ShaderDataType::Int4:    return GL_INT;
    case ShaderDataType::Bool:    return GL_BOOL;
  }

  SAMUI_ENGINE_ASSERT(false, "Unkown ShaderDataType");
  return 0;
}
// clang-format on

Application::Application(/* args */) {
  instance_ = this;
  window_ = Window::Create();
  window_->SetEventCallback(BIND_EVENT_FUNC(OnEvent));

  imgui_layer_ = new ImGuiLayer();
  PushOverlay(imgui_layer_);

  // 顶点数据
  float vertices[3*7] = {
    -0.5f, -0.5f, 0.f, 0.8f, 0.2f, 0.8f, 1.f,
  0.5f, -0.5f, 0.f, 0.2f, 0.3f, 0.8f, 1.f,
  0.f, 0.5f, 0.f, 0.8f, 0.8f, 0.2f, 1.f,
  };

  // 顶点缓冲对象
  GLuint VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // 顶点数组对象
  // GLuint VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  BufferLayout layout = {
      {"Position", ShaderDataType::Float3},
      {"Color", ShaderDataType::Float4},
  };

  uint32_t index = 0;
  for (const auto& elem : layout) {
    // 定义OpenGL如何理解该顶点数据
    glVertexAttribPointer(index, ShaderDataTypeCount(elem.Type),
                          ShaderDataTypeToOpenGLBaseType(elem.Type),
                          elem.Normalized ? GL_TRUE : GL_FALSE,
                          layout.GetStride(), (const void*)elem.Offset);

    // 启用顶点属性 顶点属性默认是禁用的
    glEnableVertexAttribArray(index);
    index++;
  }

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
    glBindVertexArray(VAO);
    // 绘制物体
    glDrawArrays(GL_TRIANGLES, 0, 3);

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
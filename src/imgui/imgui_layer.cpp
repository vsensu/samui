#include "imgui_layer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "../core/application.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace samui {

ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}
ImGuiLayer::~ImGuiLayer() {}

void ImGuiLayer::OnAttach() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      (GLFWwindow*)(Application::Get().GetWindow().GetNativeWindow()), true);
  ImGui_ImplOpenGL3_Init("#version 130");
}

void ImGuiLayer::OnDetach() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}

void ImGuiLayer::Begin() {
  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
}

void ImGuiLayer::End() {
  ImGuiIO&     io = ImGui::GetIO();
  Application& app = Application::Get();
  io.DisplaySize =
      ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

  // Rendering
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

}  // namespace samui

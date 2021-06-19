#include "imgui_layer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <imgui.h>

#include "application.h"
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
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable
  // Keyboard Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //
  // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(
      (GLFWwindow*)(Application::Get().GetWindow().GetNativeWindow()), true);
  ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiLayer::OnDetach() {}

void ImGuiLayer::OnUpdate() {
  ImGuiIO&     io = ImGui::GetIO();
  Application& app = Application::Get();
  io.DisplaySize =
      ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // Finally Render UI
  //   RenderUI();
  static bool show = true;
  ImGui::ShowDemoWindow(&show);

  // Rendering
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiLayer::OnEvent(Event& event) {}

}  // namespace samui

#include "editor_layer.h"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

namespace samui {
EditorLayer::EditorLayer()
    : Layer("EditorLayer"), camera_controller_(1280.f / 720.f, true) {}

void EditorLayer::OnAttach() {
  SAMUI_PROFILE_FUNCTION();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  camera_controller_.SetZoomLevel(5.f);

  FrameBufferSpecification spec;
  spec.width = 1280;
  spec.height = 720;
  frame_buffer_ = FrameBuffer::Create(spec);
}

void EditorLayer::OnDetach() { SAMUI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(const Timestep& deltaTime) {
  SAMUI_PROFILE_FUNCTION();
  camera_controller_.OnUpdate(deltaTime);

  Renderer2D::ResetStats();
  {
    SAMUI_PROFILE_SCOPE("Render Prepare");
    RenderCommand::Clear();
  }

  {
    SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
    frame_buffer_->Bind();
    RenderCommand::Clear();
    Renderer2D::BeginScene(camera_controller_.GetCamera());
    Renderer2D::EndScene();
    frame_buffer_->Unbind();
  }
}

void EditorLayer::OnImGuiRender() {
  auto stats = Renderer2D::GetStats();
  ImGui::Begin("Settings");
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Quads: %d", stats.quad_count);
  ImGui::ColorEdit4("square color", glm::value_ptr(square_color_));
  uint32_t texture_id = frame_buffer_->GetColorAttachmentRenderID();
  ImGui::Image((ImTextureID)texture_id, ImVec2{1280, 720});
  ImGui::End();
}

void EditorLayer::OnEvent(Event& event) { camera_controller_.OnEvent(event); }

}  // namespace samui

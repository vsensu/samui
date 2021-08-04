// clang-format off
#include "editor_layer.h"

#include <glm/gtc/type_ptr.hpp>
// clang-format on

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

  active_scene_ = std::make_shared<Scene>();

  square_entity_ = active_scene_->CreateEntity();
  active_scene_->AddComponent<TransformComponent>(square_entity_,
                                                  glm::mat4(1.f));
  active_scene_->AddComponent<SpriteRendererComponent>(
      square_entity_, glm::vec4{0.f, 1.f, 0.f, 1.f});

  // active_scene_->HasComponent<TransformComponent>(square_entity_);
}

void EditorLayer::OnDetach() { SAMUI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(const Timestep& deltaTime) {
  SAMUI_PROFILE_FUNCTION();

  if (viewport_focused_) {
    camera_controller_.OnUpdate(deltaTime);
  }

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
    active_scene_->OnUpdate(deltaTime);
    Renderer2D::EndScene();

    frame_buffer_->Unbind();
  }
}

void EditorLayer::OnImGuiRender() {
  ImGui::Begin("Main");

  auto stats = Renderer2D::GetStats();
  ImGui::Begin("Settings");
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Quads: %d", stats.quad_count);
  auto& square_color_ = active_scene_->registry()
                            .get<SpriteRendererComponent>(square_entity_)
                            .color;
  ImGui::ColorEdit4("square color", glm::value_ptr(square_color_));
  ImGui::End();

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
  ImGui::Begin("Viewport");
  viewport_focused_ = ImGui::IsWindowFocused();
  viewport_hovered_ = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!viewport_focused_ ||
                                                  !viewport_hovered_);
  const auto& viewport_size = ImGui::GetContentRegionAvail();
  if (viewport_size.x > 0 && viewport_size.y > 0) {
    if (viewport_size.x != last_viewport_size_.x ||
        viewport_size.y != last_viewport_size_.y) {
      frame_buffer_->Resize(viewport_size.x, viewport_size.y);
      camera_controller_.OnResize(viewport_size.x, viewport_size.y);
    }
  }
  last_viewport_size_ = viewport_size;
  uint32_t texture_id = frame_buffer_->GetColorAttachmentRenderID();
  // flip y
  ImGui::Image((ImTextureID)texture_id, viewport_size, {0.f, 1.f}, {1.f, 0.f});
  ImGui::End();
  ImGui::PopStyleVar();

  ImGui::End();
}

void EditorLayer::OnEvent(Event& event) { camera_controller_.OnEvent(event); }

}  // namespace samui

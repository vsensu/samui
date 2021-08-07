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

  square_entity_ = active_scene_->CreateEntity("Green Square");
  active_scene_->AddComponent<SpriteRendererComponent>(
      square_entity_, glm::vec4{0.f, 1.f, 0.f, 1.f});

  auto red_square = active_scene_->CreateEntity("Red Square");
  active_scene_->AddComponent<SpriteRendererComponent>(
      red_square, glm::vec4{1.f, 0.f, 0.f, 1.f});

  first_camera_ = active_scene_->CreateEntity("First Camera");
  active_scene_->AddComponent<CameraComponent>(first_camera_,
                                               CameraComponent{});
  // first_camera_, glm::ortho(-16.f, 16.f, -9.f, 9.f, -1.f, 1.f), 1, 1.f);

  second_camera_ = active_scene_->CreateEntity("Second Camera");
  active_scene_->AddComponent<CameraComponent>(second_camera_).projection_type =
      CameraComponent::ProjectionType::Perspective;
  // second_camera_, glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f), 1, 1.f);

  scene_hierarchy_panel_ = std::make_shared<SceneHierarchyPanel>(active_scene_);
}

void EditorLayer::OnDetach() { SAMUI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(const Timestep& deltaTime) {
  SAMUI_PROFILE_FUNCTION();

  if (viewport_focused_) {
    camera_controller_.OnUpdate(deltaTime);
  }

  // update scene logic
  active_scene_->OnUpdate(deltaTime);

  Renderer2D::ResetStats();
  {
    SAMUI_PROFILE_SCOPE("Render Prepare");
    RenderCommand::Clear();
  }

  {
    SAMUI_PROFILE_SCOPE("Render Draw(CPU)");
    frame_buffer_->Bind();
    RenderCommand::Clear();

    // Renderer2D::BeginScene(camera_controller_.GetCamera());

    // render scene
    if (main_camera_ != entt::null) {
      auto& camera_comp =
          active_scene_->GetComponent<CameraComponent>(main_camera_);
      auto projection =
          camera_comp.projection_type ==
                  CameraComponent::ProjectionType::Orthographic
              ? CameraUtils::get_orthographic_projection(
                    camera_comp.aspect_ratio, camera_comp.ortho_size,
                    camera_comp.ortho_near, camera_comp.ortho_far)
              : CameraUtils::get_perspective_projection(
                    camera_comp.aspect_ratio, camera_comp.pers_fov,
                    camera_comp.pers_near, camera_comp.pers_far);
      Renderer2D::BeginScene(
          projection,
          // active_scene_->GetComponent<CameraComponent>(main_camera_).projection,
          active_scene_->GetComponent<TransformComponent>(main_camera_)
              .transform());
      auto view = active_scene_->registry()
                      .view<TransformComponent, SpriteRendererComponent>();
      view.each([](const auto& transform, const auto& sprite) {
        Renderer2D::DrawQuad(transform.transform(), sprite.color);
      });
      Renderer2D::EndScene();
    }

    // Renderer2D::EndScene();

    frame_buffer_->Unbind();
  }
}

void EditorLayer::OnImGuiRender() {
  OnImGuiFullScreenDocking();

  // settings pannel begine
  auto stats = Renderer2D::GetStats();
  ImGui::Begin("Stats");
  ImGui::Text("Renderer2D Stats:");
  ImGui::Text("Draw Calls: %d", stats.draw_calls);
  ImGui::Text("Quads: %d", stats.quad_count);
  ImGui::Separator();

  auto cameras =
      active_scene_->registry()
          .view<CameraComponent, NameComponent, TransformComponent>();
  static int          camera_index = 0;
  auto                items = new const char*[cameras.size_hint()];
  std::vector<Entity> camera_entities;
  int                 index = 0;
  cameras.each([&](const auto& camera, const auto& name,
                   const auto& transform) {
    items[index++] = name.name.c_str();
    camera_entities.push_back(entt::to_entity(active_scene_->registry(), name));
  });

  ImGui::Combo("Main Camera", &camera_index, items, index);
  main_camera_ = camera_entities[camera_index];
  ImGui::End();
  // settings pannel end
  delete[] items;

  // scene pannel begin
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
      auto& camera_comp =
          active_scene_->GetComponent<CameraComponent>(main_camera_);
      if (!camera_comp.fixed_aspect_ratio) {
        camera_comp.aspect_ratio = viewport_size.x / viewport_size.y;
      }
      // main_camera_.OnResize();
    }
  }
  last_viewport_size_ = viewport_size;
  uint32_t texture_id = frame_buffer_->GetColorAttachmentRenderID();
  // flip y
  ImGui::Image((ImTextureID)texture_id, viewport_size, {0.f, 1.f}, {1.f, 0.f});
  ImGui::End();
  ImGui::PopStyleVar();
  // scene pannel end

  // scene hierarchy pannel begin
  scene_hierarchy_panel_->OnImGuiRender();
  // scene hierarchy pannel end
}

void EditorLayer::OnEvent(Event& event) { camera_controller_.OnEvent(event); }

void EditorLayer::OnImGuiFullScreenDocking() {
  static bool               p_open = true;
  static bool               opt_fullscreen = true;
  static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
  if (opt_fullscreen) {
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  } else {
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render
  // our background and handle the pass-thru hole, so we ask Begin() to not
  // render a background.
  if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
    window_flags |= ImGuiWindowFlags_NoBackground;

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  ImGui::Begin("DockSpace Demo", &p_open, window_flags);
  if (opt_fullscreen) ImGui::PopStyleVar(2);

  // Submit the DockSpace
  ImGuiIO& io = ImGui::GetIO();
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
      if (ImGui::MenuItem("Exit")) {
        Application::Get().Close();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

}  // namespace samui

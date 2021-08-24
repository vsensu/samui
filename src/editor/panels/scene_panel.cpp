// clang-format off
#include "scene_panel.h"

#include <editor_layer.h>
// clang-format on

namespace samui {
ScenePanel::ScenePanel(const Ref<Scene>& scene, EditorLayer* parent)
    : scene_(scene), parent_(parent) {
  FrameBufferSpecification spec;
  spec.attachments = {
      FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RED_INTEGER,
      FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA,
      FrameBufferTextureFormat::Depth};
  spec.width = 1280;
  spec.height = 720;
  frame_buffer_ = FrameBuffer::Create(spec);

  viewport_bounds_[0] = {0.f, 0.f};
  viewport_bounds_[1] = {0.f, 0.f};

  editor_camera_ = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
}

void ScenePanel::SetScene(const Ref<Scene>& scene) {
  scene_ = scene;
  hovered_entity_ = entt::null;
}

void ScenePanel::OnUpdate(const Timestep& deltaTime) {
  editor_camera_.OnUpdate(deltaTime);
}

void ScenePanel::RenderScene() {
  SAMUI_PROFILE_FUNCTION();
  frame_buffer_->Bind();
  RenderCommand::Clear();
  frame_buffer_->ClearAttachment(1, static_cast<uint32_t>(entt::null));

  // Renderer2D::BeginScene(camera_controller_.GetCamera());

  // render scene
  // if (main_camera_ != entt::null) {
  // auto& camera_comp =
  //     active_scene_->GetComponent<CameraComponent>(main_camera_);
  // auto projection =
  //     camera_comp.projection_type ==
  //             CameraComponent::ProjectionType::Orthographic
  //         ? CameraUtils::get_orthographic_projection(
  //               camera_comp.aspect_ratio, camera_comp.ortho_size,
  //               camera_comp.ortho_near, camera_comp.ortho_far)
  //         : CameraUtils::get_perspective_projection(
  //               camera_comp.aspect_ratio, camera_comp.pers_fov,
  //               camera_comp.pers_near, camera_comp.pers_far);
  Renderer2D::BeginScene(editor_camera_.GetViewProjection());
  // projection,
  // active_scene_->GetComponent<TransformComponent>(main_camera_)
  // .transform());
  auto group =
      scene_->registry().group<TransformComponent, SpriteRendererComponent>();
  for (auto entity : group) {
    auto& transform = group.get<TransformComponent>(entity);
    auto& sprite = group.get<SpriteRendererComponent>(entity);
    Renderer2D::DrawSprite(transform.transform(), sprite, entity);
  }

  Renderer2D::EndScene();
  // }

  // Renderer2D::EndScene();

  auto [mx, my] = ImGui::GetMousePos();
  mx -= viewport_bounds_[0].x;
  my -= viewport_bounds_[0].y;
  glm::vec2 viewport_size = viewport_bounds_[1] - viewport_bounds_[0];
  my = viewport_size.y - my;
  int mouse_x = (int)mx;
  int mouse_y = (int)my;

  if (mouse_x >= 0 && mouse_y >= 0 && mouse_x < viewport_size.x &&
      mouse_y < viewport_size.y) {
    int pixel = frame_buffer_->ReadPixel(1, mouse_x, mouse_y);
    hovered_entity_ = (pixel == -1 ? entt::null : (Entity)pixel);
  }

  frame_buffer_->Unbind();
}

void ScenePanel::OnImGuiRender() {
  // scene pannel begin
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
  ImGui::Begin("Viewport");
  // auto viewport_offset = ImGui::GetCursorPos();
  auto viewport_min_region = ImGui::GetWindowContentRegionMin();
  auto viewport_max_region = ImGui::GetWindowContentRegionMax();
  auto viewport_offset = ImGui::GetWindowPos();
  viewport_bounds_[0] = {viewport_min_region.x + viewport_offset.x,
                         viewport_min_region.y + viewport_offset.y};
  viewport_bounds_[1] = {viewport_max_region.x + viewport_offset.x,
                         viewport_max_region.y + viewport_offset.y};

  viewport_focused_ = ImGui::IsWindowFocused();
  viewport_hovered_ = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!viewport_focused_ &&
                                                  !viewport_hovered_);
  const auto&   viewport_size = ImGui::GetContentRegionAvail();
  static ImVec2 last_viewport_size_;
  if (viewport_size.x > 0 && viewport_size.y > 0) {
    if (viewport_size.x != last_viewport_size_.x ||
        viewport_size.y != last_viewport_size_.y) {
      frame_buffer_->Resize(viewport_size.x, viewport_size.y);
      //   camera_controller_.OnResize(viewport_size.x, viewport_size.y);
      //   if (main_camera_ != entt::null) {
      //     auto& camera_comp =
      //         active_scene_->GetComponent<CameraComponent>(main_camera_);
      //     if (!camera_comp.fixed_aspect_ratio) {
      //       camera_comp.aspect_ratio = viewport_size.x / viewport_size.y;
      //     }
      //   }
      // main_camera_.OnResize();
      editor_camera_.SetViewportSize(viewport_size.x, viewport_size.y);
    }
  }
  last_viewport_size_ = viewport_size;
  uint32_t texture_id = frame_buffer_->GetColorAttachmentRenderID();
  // flip y
  ImGui::Image((ImTextureID)texture_id, viewport_size, {0.f, 1.f}, {1.f, 0.f});

  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload =
            ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
      const char* path = (const char*)payload->Data;
      parent_->OpenScene(path);
    }

    ImGui::EndDragDropTarget();
  }

  // Draw Gizmos
  Entity selected_entity = parent_->GetSelectedEntity();
  if (selected_entity != entt::null) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(viewport_bounds_[0].x, viewport_bounds_[0].y,
                      viewport_bounds_[1].x - viewport_bounds_[0].x,
                      viewport_bounds_[1].y - viewport_bounds_[0].y);

    // Camera Runtime
    // const auto& camera_comp =
    // active_scene_->GetComponent<CameraComponent>(main_camera_);
    // const auto& projection = CameraUtils::get_perspective_projection(
    // camera_comp.aspect_ratio, camera_comp.pers_fov, camera_comp.pers_near,
    // camera_comp.pers_far);
    // glm::mat4 camera_view = glm::inverse(
    // active_scene_->GetComponent<TransformComponent>(main_camera_)
    // .transform());

    // Editor camera
    const glm::mat4& projection = editor_camera_.GetProjection();
    glm::mat4        camera_view = editor_camera_.GetViewMatrix();

    // Entity
    auto& trans_comp =
        scene_->GetComponent<TransformComponent>(selected_entity);
    glm::mat4 transform = trans_comp.transform();

    bool  snap = Input::IsKeyPressed(SAMUI_KEY_LEFT_CONTROL);
    float snap_value = gizmos_op_ == ImGuizmo::OPERATION::ROTATE ? 45.f : 0.5f;
    float snap_values[3] = {snap_value, snap_value, snap_value};

    ImGuizmo::Manipulate(glm::value_ptr(camera_view),
                         glm::value_ptr(projection), gizmos_op_,
                         ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr,
                         snap ? snap_values : nullptr);

    if (ImGuizmo::IsUsing()) {
      glm::vec3 translation, rotation, scale;
      ImGuizmo::DecomposeMatrixToComponents(
          glm::value_ptr(transform), glm::value_ptr(translation),
          glm::value_ptr(rotation), glm::value_ptr(scale));

      if (gizmos_op_ == ImGuizmo::OPERATION::TRANSLATE) {
        trans_comp.translation = translation;
      } else if (gizmos_op_ == ImGuizmo::OPERATION::ROTATE) {
        trans_comp.rotation = glm::radians(rotation);
      } else if (gizmos_op_ == ImGuizmo::OPERATION::SCALE) {
        trans_comp.scale = scale;
      }
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();
  // scene pannel end
}

void ScenePanel::OnEvent(Event& event) { editor_camera_.OnEvent(event); }

}  // namespace samui
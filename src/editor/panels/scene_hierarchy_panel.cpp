#include "scene_hierarchy_panel.h"

#include <glm/gtc/type_ptr.hpp>

namespace samui {
SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
    : scene_(scene) {}

void SceneHierarchyPanel::OnImGuiRender() {
  ImGui::Begin("Scene Hierarchy");

  scene_->registry().each([&](auto entity) {
    auto               name = scene_->GetComponent<NameComponent>(entity).name;
    ImGuiTreeNodeFlags flags =
        (selected_entity_ == entity ? ImGuiTreeNodeFlags_Selected : 0) |
        ImGuiTreeNodeFlags_OpenOnArrow;
    bool opened = ImGui::TreeNodeEx((void*)entity, flags, name.c_str());
    if (ImGui::IsItemClicked()) {
      selected_entity_ = entity;
    }

    if (opened) {
      ImGui::TreePop();
    }
  });

  if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
    selected_entity_ = entt::null;
  }

  ImGui::End();

  ImGui::Begin("Inspector");
  if (selected_entity_ != entt::null) {
    // Draw Property
    if (scene_->HasComponent<NameComponent>(selected_entity_)) {
      auto& name_comp = scene_->GetComponent<NameComponent>(selected_entity_);
      char  buff[256];
      memset(buff, 0, sizeof(buff));
      strcpy(buff, name_comp.name.c_str());
      if (ImGui::InputText("Name", buff, sizeof(buff))) {
        name_comp.name = buff;
      }
    }

    if (scene_->HasComponent<TransformComponent>(selected_entity_)) {
      auto& transform_comp =
          scene_->GetComponent<TransformComponent>(selected_entity_);
      ImGui::DragFloat3("Position", glm::value_ptr(transform_comp.transform[3]),
                        0.1f);
    }

    if (scene_->HasComponent<SpriteRendererComponent>(selected_entity_)) {
      auto& sprite_comp =
          scene_->GetComponent<SpriteRendererComponent>(selected_entity_);
      ImGui::ColorEdit4("Sprite Color", glm::value_ptr(sprite_comp.color));
    }

    if (scene_->HasComponent<CameraComponent>(selected_entity_)) {
      auto& camera_comp =
          scene_->GetComponent<CameraComponent>(selected_entity_);
      if (camera_comp.projection_type ==
          CameraComponent::ProjectionType::Orthographic) {
        ImGui::Text("Orthographic");
        ImGui::DragFloat("Size", &camera_comp.ortho_size);
        ImGui::DragFloat("Near Clip", &camera_comp.ortho_near);
        ImGui::DragFloat("Far Clip", &camera_comp.ortho_far);
      } else if (camera_comp.projection_type ==
                 CameraComponent::ProjectionType::Perspective) {
        ImGui::Text("Perspective");
        ImGui::DragFloat("FOV", &camera_comp.pers_fov);
        ImGui::DragFloat("Near Clip", &camera_comp.pers_near);
        ImGui::DragFloat("Far Clip", &camera_comp.pers_far);
      }
      ImGui::Checkbox("Fixed Aspect Ratio", &camera_comp.fixed_aspect_ratio);
    }
  }

  ImGui::End();
}

}  // namespace samui

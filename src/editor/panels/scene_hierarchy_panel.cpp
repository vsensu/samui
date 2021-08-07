#include "scene_hierarchy_panel.h"

#include <glm/gtc/type_ptr.hpp>

namespace samui {
void DrawVec3Control(const std::string& label, glm::vec3& values,
                     float reset_value = 0.f, float column_width = 100.f) {
  ImGui::PushID(label.c_str());
  ImGui::Columns(2);

  ImGui::SetColumnWidth(0, column_width);
  ImGui::Text(label.c_str());

  ImGui::NextColumn();
  ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

  float line_height =
      GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
  ImVec2 button_size = {line_height + 3.f, line_height};

  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
  if (ImGui::Button("X", button_size)) {
    values.x = reset_value;
  }
  ImGui::PopStyleColor(3);
  ImGui::SameLine();
  ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
  if (ImGui::Button("Y", button_size)) {
    values.y = reset_value;
  }
  ImGui::PopStyleColor(3);
  ImGui::SameLine();
  ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::SameLine();
  ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                        ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
  if (ImGui::Button("Z", button_size)) {
    values.z = reset_value;
  }
  ImGui::PopStyleColor(3);
  ImGui::SameLine();
  ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.2f");
  ImGui::PopItemWidth();

  ImGui::PopStyleVar();
  ImGui::Columns(1);
  ImGui::PopID();
}

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
      DrawVec3Control("Position", transform_comp.translation);
      auto rotation = glm::degrees(transform_comp.rotation);
      DrawVec3Control("Rotation", rotation);
      transform_comp.rotation = glm::radians(rotation);
      DrawVec3Control("Scale", transform_comp.scale, 1.f);
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

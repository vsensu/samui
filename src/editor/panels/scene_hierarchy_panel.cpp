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

  if (selected_entity_ == entt::null) {
    // Right-click on blank space
    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
      if (ImGui::MenuItem("Create Empty Entity")) {
        scene_->CreateEntity("Empty Entity");
      }
      ImGui::EndPopup();
    }
  }

  else {
    // Right click on entity
    if (ImGui::BeginPopupContextWindow()) {
      if (ImGui::MenuItem("Delete Entity")) {
        scene_->DestroyEntity(selected_entity_);
        selected_entity_ = entt::null;
      }
      ImGui::EndPopup();
    }
  }
  ImGui::End();

  ImGui::Begin("Inspector");
  if (selected_entity_ != entt::null) {
    DrawProperties(selected_entity_);

    if (ImGui::Button("Add Component")) {
      ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent")) {
      if (ImGui::MenuItem("Camera")) {
        scene_->AddComponent<CameraComponent>(selected_entity_);
        ImGui::CloseCurrentPopup();
      }

      if (ImGui::MenuItem("Sprite Renderer")) {
        scene_->AddComponent<SpriteRendererComponent>(selected_entity_);
        ImGui::CloseCurrentPopup();
      }

      ImGui::EndPopup();
    }
  }

  ImGui::End();
}

void SceneHierarchyPanel::DrawProperties(Entity entity) {
  const ImGuiTreeNodeFlags treeNodeFlags =
      ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

  // Draw Property
  if (scene_->HasComponent<NameComponent>(entity)) {
    auto& name_comp = scene_->GetComponent<NameComponent>(entity);
    char  buff[256];
    memset(buff, 0, sizeof(buff));
    strcpy(buff, name_comp.name.c_str());
    if (ImGui::InputText("Name", buff, sizeof(buff))) {
      name_comp.name = buff;
    }
  }

  if (scene_->HasComponent<TransformComponent>(entity)) {
    bool open = ImGui::TreeNodeEx((void*)typeid(entity).hash_code(),
                                  treeNodeFlags, "Transform");
    if (open) {
      auto& transform_comp = scene_->GetComponent<TransformComponent>(entity);
      DrawVec3Control("Position", transform_comp.translation);
      auto rotation = glm::degrees(transform_comp.rotation);
      DrawVec3Control("Rotation", rotation);
      transform_comp.rotation = glm::radians(rotation);
      DrawVec3Control("Scale", transform_comp.scale, 1.f);

      ImGui::TreePop();
    }
  }

  if (scene_->HasComponent<CameraComponent>(entity)) {
    bool open = ImGui::TreeNodeEx((void*)typeid(entity).hash_code(),
                                  treeNodeFlags, "Camera");
    ImGui::SameLine();
    if (ImGui::Button("...")) {
      ImGui::OpenPopup("ComponentSettings");
    }
    bool remove_component = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove Component")) {
        bool remove_component = true;
      }
      ImGui::EndPopup();
    }

    if (open) {
      auto& camera_comp = scene_->GetComponent<CameraComponent>(entity);
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

      ImGui::TreePop();
    }

    if (remove_component) {
      scene_->RemoveComponent<CameraComponent>(entity);
    }
  }

  if (scene_->HasComponent<SpriteRendererComponent>(entity)) {
    bool open = ImGui::TreeNodeEx((void*)typeid(entity).hash_code(),
                                  treeNodeFlags, "Sprite Renderer");
    ImGui::SameLine();
    if (ImGui::Button("...")) {
      ImGui::OpenPopup("ComponentSettings");
    }
    bool remove_component = false;
    if (ImGui::BeginPopup("ComponentSettings")) {
      if (ImGui::MenuItem("Remove Component")) {
        remove_component = true;
      }
      ImGui::EndPopup();
    }

    if (open) {
      auto& sprite_comp = scene_->GetComponent<SpriteRendererComponent>(entity);
      ImGui::ColorEdit4("Sprite Color", glm::value_ptr(sprite_comp.color));

      ImGui::TreePop();
    }

    if (remove_component) {
      scene_->RemoveComponent<SpriteRendererComponent>(entity);
    }
  }
}

}  // namespace samui

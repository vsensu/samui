#include "scene_hierarchy_panel.h"

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
      auto &name_comp = scene_->GetComponent<NameComponent>(selected_entity_);
      char buff[256];
      memset(buff, 0, sizeof(buff));
      strcpy(buff, name_comp.name.c_str());
      if(ImGui::InputText("Name", buff, sizeof(buff)))
      {
          name_comp.name = buff;
      }
    }
  }

  ImGui::End();
}

}  // namespace samui

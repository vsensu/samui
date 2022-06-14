// clang-format off
#include "scene_hierarchy_panel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>

#include <engine/graphics/renderer/texture_create.h>

#include "../scene/components.h"
// clang-format on

namespace samui
{
void DrawVec3Control(const std::string& label, glm::vec3& values,
                     float reset_value = 0.f, float column_width = 100.f)
{
    ImGuiIO& io = ImGui::GetIO();
    auto     bold_font = io.Fonts->Fonts[0];

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
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
    ImGui::PushFont(bold_font);
    if (ImGui::Button("X", button_size))
    {
        values.x = reset_value;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##X", &values.x, 0.1f, 0.f, 0.f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Y", button_size))
    {
        values.y = reset_value;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Y", &values.y, 0.1f, 0.f, 0.f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::SameLine();
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
    ImGui::PushFont(bold_font);
    if (ImGui::Button("Z", button_size))
    {
        values.z = reset_value;
    }
    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    ImGui::SameLine();
    ImGui::DragFloat("##Z", &values.z, 0.1f, 0.f, 0.f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();
    ImGui::Columns(1);
    ImGui::PopID();
}

template <typename T, typename UIFunction>
void DrawComponent(const std::string& label, Scene& scene, Entity entity,
                   const UIFunction& func)
{
    if (scene.has_component<T>(entity))
    {
        constexpr ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_FramePadding | ImGuiTreeNodeFlags_SpanAvailWidth;
        ImVec2 contentRegion = ImGui::GetContentRegionAvail();

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, {4.f, 4.f});
        float line_height =
            GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.f;
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(),
                                      treeNodeFlags, label.c_str());
        ImGui::PopStyleVar();
        ImGui::SameLine(contentRegion.x - line_height * 0.5f);
        if (ImGui::Button("...", {line_height, line_height}))
        {
            ImGui::OpenPopup("ComponentSettings");
        }
        bool remove_component = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            if (ImGui::MenuItem("Remove Component"))
            {
                remove_component = true;
            }
            ImGui::EndPopup();
        }

        if (open)
        {
            auto& comp = scene.get_component<T>(entity);
            func(comp);

            ImGui::TreePop();
        }

        if (remove_component)
        {
            scene.remove_component<T>(entity);
        }
    }
}

SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<Scene>& scene)
    : scene_(scene)
{
}

void SceneHierarchyPanel::OnImGuiRender()
{
    ImGui::Begin("Scene Hierarchy");

    scene_->registry().each(
        [&](auto entity)
        {
            auto name = scene_->get_component<NameComponent>(entity).name;
            ImGuiTreeNodeFlags flags =
                (selected_entity_ == entity ? ImGuiTreeNodeFlags_Selected : 0) |
                ImGuiTreeNodeFlags_OpenOnArrow;
            flags |=
                ImGuiTreeNodeFlags_SpanAvailWidth;  // click item empty space
                                                    // can select too.
            bool opened = ImGui::TreeNodeEx((void*)entity, flags, name.c_str());
            if (ImGui::IsItemClicked())
            {
                selected_entity_ = entity;
            }

            if (opened)
            {
                ImGui::TreePop();
            }
        });

    if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
    {
        selected_entity_ = entt::null;
    }

    if (selected_entity_ == entt::null)
    {
        // Right-click on blank space
        if (ImGui::BeginPopupContextWindow(0, 1, false))
        {
            if (ImGui::MenuItem("Create Empty Entity"))
            {
                scene_->create_entity("Empty Entity");
            }

            if (ImGui::MenuItem("Create Camera"))
            {
                auto camera_entity = scene_->create_entity("Camera");
                scene_->add_component<CameraComponent>(camera_entity);
            }

            if (ImGui::MenuItem("Create Sprite"))
            {
                auto sprite_entity = scene_->create_entity("Sprite");
                scene_->add_component<SpriteRendererComponent>(sprite_entity);
            }
            ImGui::EndPopup();
        }
    }

    else
    {
        // Right click on entity
        if (ImGui::BeginPopupContextWindow())
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                scene_->destroy_entity(selected_entity_);
                selected_entity_ = entt::null;
            }
            ImGui::EndPopup();
        }
    }
    ImGui::End();

    ImGui::Begin("Inspector");
    if (selected_entity_ != entt::null)
    {
        DrawProperties(selected_entity_);
    }

    ImGui::End();
}

void SceneHierarchyPanel::SetScene(std::shared_ptr<Scene> scene)
{
    scene_ = scene;
    selected_entity_ = entt::null;
}

void SceneHierarchyPanel::DrawProperties(Entity entity)
{
    const ImGuiTreeNodeFlags treeNodeFlags =
        ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

    // Draw Property
    if (scene_->has_component<NameComponent>(entity))
    {
        auto& name_comp = scene_->get_component<NameComponent>(entity);
        char  buff[256];
        memset(buff, 0, sizeof(buff));
        strcpy(buff, name_comp.name.c_str());
        if (ImGui::InputText("##Name", buff, sizeof(buff)))
        {
            name_comp.name = buff;
        }
    }

    ImGui::SameLine();
    ImGui::PushItemWidth(-1.f);
    if (ImGui::Button("Add Component"))
    {
        ImGui::OpenPopup("AddComponent");
    }

    if (ImGui::BeginPopup("AddComponent"))
    {
        if (ImGui::MenuItem("Camera"))
        {
            scene_->add_component<CameraComponent>(selected_entity_);
            ImGui::CloseCurrentPopup();
        }

        if (ImGui::MenuItem("Sprite Renderer"))
        {
            scene_->add_component<SpriteRendererComponent>(selected_entity_);
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();  // End "AddComponent" popup
    }
    ImGui::PopItemWidth();

    DrawComponent<TransformComponent>(
        "Transform", *scene_, entity,
        [](auto& component)
        {
            DrawVec3Control("Position", component.translation);
            auto rotation = glm::degrees(component.rotation);
            DrawVec3Control("Rotation", rotation);
            component.rotation = glm::radians(rotation);
            DrawVec3Control("Scale", component.scale, 1.f);
        });

    DrawComponent<CameraComponent>(
        "Camera", *scene_, entity,
        [](auto& component)
        {
            if (component.projection_type ==
                CameraComponent::ProjectionType::Orthographic)
            {
                ImGui::Text("Orthographic");
                ImGui::DragFloat("Size", &component.ortho_size);
                ImGui::DragFloat("Near Clip", &component.ortho_near);
                ImGui::DragFloat("Far Clip", &component.ortho_far);
            }
            else if (component.projection_type ==
                     CameraComponent::ProjectionType::Perspective)
            {
                ImGui::Text("Perspective");
                ImGui::DragFloat("FOV", &component.pers_fov);
                ImGui::DragFloat("Near Clip", &component.pers_near);
                ImGui::DragFloat("Far Clip", &component.pers_far);
            }
            ImGui::Checkbox("Fixed Aspect Ratio",
                            &component.fixed_aspect_ratio);
        });

    DrawComponent<SpriteRendererComponent>(
        "Sprite Renderer", *scene_, entity,
        [](auto& component)
        {
            ImGui::ColorEdit4("Sprite Color", glm::value_ptr(component.color));
            if (component.texture != nullptr)
            {
                ImGui::Image((ImTextureID)component.texture->get_texture_id(),
                             ImVec2(128, 128), {0, 1}, {1, 0});
            }
            else
            {
                ImGui::Image((ImTextureID)0, ImVec2(128, 128), {0, 1}, {1, 0});
            }

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload =
                        ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                {
                    const wchar_t* path = (const wchar_t*)payload->Data;
                    component.texture = samui::texture2d::create(path);
                }

                ImGui::EndDragDropTarget();
            }

            ImGui::DragFloat("Tiling Factor", &component.tiling_factor, 0.1f,
                             0.f, 100.f);
        });
}

}  // namespace samui

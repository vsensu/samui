// clang-format off
#include "editor_layer.h"

#include <engine/profiler/instrumentor.h>
#include <engine/graphics/renderer/renderer2d.h>
#include <engine/graphics/utils/platform_utils.h>
#include <engine/core/engine.h>

#include "scene/components.h"
#include "scene/serialization.h"
// clang-format on

namespace samui
{
EditorLayer::EditorLayer()
    : Layer("EditorLayer"), camera_controller_(1280.f / 720.f, true)
{
}

void EditorLayer::on_attach()
{
    SAMUI_PROFILE_FUNCTION();
    RenderCommand::set_clear_color({0.1f, 0.1f, 0.1f, 1});
    RenderCommand::set_flip_vertically_on_load(true);
    RenderCommand::set_depth_test_enable(true);
    camera_controller_.set_zoom_level(5.f);

    active_scene_ = std::make_shared<Scene>();

    scene_hierarchy_panel_ =
        std::make_shared<SceneHierarchyPanel>(active_scene_);
    content_browser_ = std::make_shared<ContentBrowser>();
    scene_panel_ = std::make_shared<ScenePanel>(active_scene_, this);

    panels_[SpriteAtlasPanel::key()] = std::make_shared<SpriteAtlasPanel>();
}

void EditorLayer::on_detach() { SAMUI_PROFILE_FUNCTION(); }

void EditorLayer::on_update(const Timestep& deltaTime)
{
    SAMUI_PROFILE_FUNCTION();
    Renderer2D::reset_stats();

    // update scene logic
    active_scene_->on_update(deltaTime);
    scene_panel_->OnUpdate(deltaTime);

    for(auto& [key, panel]: panels_)
    {
        if(panels_data_[key].visible)
        {
            panel->on_update(deltaTime);
        }
    }

    // RenderCommand::Clear();
    scene_panel_->RenderScene();
}

void EditorLayer::on_imgui_render()
{
    OnImGuiFullScreenDocking();

    scene_panel_->OnImGuiRender();
    for (auto& panel : panels_)
    {
        bool &visible = panels_data_[panel.first].visible;
        if (visible)
        {
            ImGui::Begin(panel.second->name().c_str(), &visible);
            panel.second->on_imgui_render();
            ImGui::End();
        }
    }

    // settings pannel begine
    auto stats = Renderer2D::get_stats();
    ImGui::Begin("Stats");
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.draw_calls);
    ImGui::Text("Quads: %d", stats.quad_count);
    ImGui::Separator();

    auto cameras =
        active_scene_->registry()
            .view<CameraComponent, NameComponent, TransformComponent>();
    if (cameras.size_hint() > 0)
    {
        static int          camera_index = 0;
        auto                items = new const char*[cameras.size_hint()];
        std::vector<Entity> camera_entities;
        int                 index = 0;
        cameras.each(
            [&](const auto& camera, const auto& name, const auto& transform)
            {
                items[index++] = name.name.c_str();
                camera_entities.push_back(
                    entt::to_entity(active_scene_->registry(), name));
            });

        ImGui::Combo("Main Camera", &camera_index, items, index);
        main_camera_ = camera_entities[camera_index];
        delete[] items;
    }
    ImGui::End();  // settings pannel end

    // scene hierarchy pannel begin
    scene_hierarchy_panel_->OnImGuiRender();
    // scene hierarchy pannel end

    content_browser_->OnImGuiRender();
}

void EditorLayer::on_event(Event& event)
{
    scene_panel_->OnEvent(event);
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<KeyPressedEvent>(
        BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
    dispatcher.dispatch<MouseButtonPressedEvent>(
        BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));
}

void EditorLayer::OnImGuiFullScreenDocking()
{
    static bool               p_open = true;
    static bool               opt_fullscreen = true;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent
    // window not dockable into, because it would be confusing to have two
    // docking targets within each others.
    ImGuiWindowFlags window_flags =
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen)
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar |
                        ImGuiWindowFlags_NoCollapse |
                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will
    // render our background and handle the pass-thru hole, so we ask Begin() to
    // not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window
    // is collapsed). This is because we want to keep our DockSpace() active. If
    // a DockSpace() is inactive, all active windows docked into it will lose
    // their parent and become undocked. We cannot preserve the docking
    // relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in
    // limbo and never being visible.
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);
    if (opt_fullscreen) ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiIO& io = ImGui::GetIO();
    auto&    style = ImGui::GetStyle();
    float    min_size_x = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = min_size_x;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            // Disabling fullscreen would allow the window to be moved to the
            // front of other windows, which we can't undo at the moment without
            // finer window depth/z control. ImGui::MenuItem("Fullscreen", NULL,
            // &opt_fullscreen);

            if (ImGui::MenuItem("New", "Ctrl+N"))
            {
                NewScene();
            }

            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                OpenScene();
            }

            if (ImGui::MenuItem("Save", "Ctrl+Shift+S"))
            {
                SaveSceneAs();
            }

            if (ImGui::MenuItem("Exit"))
            {
                Engine::instance().app()->close();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Panel"))
        {
            if (ImGui::MenuItem("Sprite Atlas Panel"))
            {
                panels_data_[SpriteAtlasPanel::key()].visible = true;
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void EditorLayer::NewScene()
{
    main_camera_ = entt::null;
    active_scene_ = std::make_shared<Scene>();
    scene_hierarchy_panel_->SetScene(active_scene_);
    scene_panel_->SetScene(active_scene_);
}

void EditorLayer::OpenScene()
{
    auto filepath = DialogUtils::open_file("Samui Scene(*.samui)\0*.samui\0");
    if (!filepath.empty())
    {
        OpenScene(filepath);
    }
}

void EditorLayer::OpenScene(const std::filesystem::path& path)
{
    main_camera_ = entt::null;
    active_scene_ = std::make_shared<Scene>();
    scene_hierarchy_panel_->SetScene(active_scene_);
    scene_panel_->SetScene(active_scene_);

    Serialization::deserialize_scene(*active_scene_, path.string());
}

void EditorLayer::SaveSceneAs()
{
    auto filepath = DialogUtils::save_file("Samui Scene(*.samui)\0*.samui\0");
    if (!filepath.empty())
    {
        Serialization::serialize_scene(*active_scene_, filepath);
    }
}

Entity EditorLayer::GetSelectedEntity()
{
    return scene_hierarchy_panel_->GetSelectedEntity();
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& event)
{
    if (event.get_repeat_count() > 0) return false;

    bool ctrl_pressed = Input::is_key_pressed(SAMUI_KEY_LEFT_CONTROL) ||
                        Input::is_key_pressed(SAMUI_KEY_RIGHT_CONTROL);

    bool shift_pressed = Input::is_key_pressed(SAMUI_KEY_LEFT_SHIFT) ||
                         Input::is_key_pressed(SAMUI_KEY_RIGHT_SHIFT);

    switch (event.get_key_code())
    {
        case SAMUI_KEY_N:
        {
            if (ctrl_pressed)
            {
                NewScene();
            }
        }
        break;
        case SAMUI_KEY_O:
        {
            if (ctrl_pressed)
            {
                OpenScene();
            }
        }
        break;
        case SAMUI_KEY_S:
        {
            if (ctrl_pressed && shift_pressed)
            {
                SaveSceneAs();
            }
        }
        break;
        case SAMUI_KEY_W:
        {
            scene_panel_->SetGizmosOp(ImGuizmo::OPERATION::TRANSLATE);
        }
        break;
        case SAMUI_KEY_E:
        {
            scene_panel_->SetGizmosOp(ImGuizmo::OPERATION::ROTATE);
        }
        break;
        case SAMUI_KEY_R:
        {
            scene_panel_->SetGizmosOp(ImGuizmo::OPERATION::SCALE);
        }
        break;
        default:
            return false;
    }

    return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event)
{
    if (scene_panel_->GetHoveredEntity() != entt::null && !ImGuizmo::IsOver() &&
        !Input::is_key_pressed(SAMUI_KEY_LEFT_CONTROL))
    {
        scene_hierarchy_panel_->SetSelectedEntity(
            scene_panel_->GetHoveredEntity());
        return false;
    }

    return true;
}

}  // namespace samui

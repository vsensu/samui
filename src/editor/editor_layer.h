#pragma once

// clang-format off
#include <memory>
#include <filesystem>
#include <list>
#include <unordered_map>

// #include <samui.h>
#include <engine/core/minimal.h>
#include <engine/core/events/key_event.h>
#include <engine/graphics/orthographic_camera_controller.h>

#include <panels/scene_hierarchy_panel.h>
#include <panels/content_browser.h>
#include <panels/scene_panel.h>
#include "panels/sprite_atlas_panel.h"
// #include "editor_camera.h"
// clang-format on

namespace samui
{
class EditorLayer : public Layer
{
public:
    EditorLayer();

    virtual void on_attach() override;
    virtual void on_detach() override;
    virtual void on_update(const Timestep& deltaTime) override;
    virtual void on_imgui_render() override;
    virtual void on_event(Event& event) override;

    void NewScene();
    void OpenScene();
    void OpenScene(const std::filesystem::path& path);
    void SaveSceneAs();

    Entity GetSelectedEntity();

    void register_panel();

private:
    void OnImGuiFullScreenDocking();

    bool OnKeyPressed(KeyPressedEvent& event);
    bool OnMouseButtonPressed(MouseButtonPressedEvent& event);

    void open_panel();

private:
    OrthographicCameraController camera_controller_;

    std::shared_ptr<Scene> active_scene_;

    Entity square_entity_;
    Entity main_camera_{entt::null};
    Entity first_camera_{entt::null};
    Entity second_camera_{entt::null};

    std::shared_ptr<SceneHierarchyPanel> scene_hierarchy_panel_;
    std::shared_ptr<ScenePanel>          scene_panel_;

    struct PanelData
    {
        bool visible;
    };

    std::unordered_map<std::string, std::shared_ptr<Panel>> panels_;
    std::unordered_map<std::string, PanelData>              panels_data_;
    std::string                                             project_path_;
};

}  // namespace samui

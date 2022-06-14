#pragma once

// clang-format off
#include <memory>

// #include <samui.h>

#include <engine/core/minimal.h>
#include <engine/rendering/frame_buffer.h>
#include <engine/graphics/renderer/render_command.h>
#include <imgui/imgui.h>
#include <engine/graphics/ImGuizmo/ImGuizmo.h>

#include "../editor_camera.h"
#include "../scene/scene.h"
// clang-format on

namespace samui
{

class EditorLayer;
class ScenePanel
{
  public:
    ScenePanel(const std::shared_ptr<Scene>& scene, EditorLayer* parent);

    void SetScene(const std::shared_ptr<Scene>& scene);

    void OnUpdate(const Timestep& deltaTime);
    void RenderScene();
    void OnImGuiRender();
    void OnEvent(Event& event);

    Entity              GetHoveredEntity() const { return hovered_entity_; }
    ImGuizmo::OPERATION GetGizmosOp() const { return gizmos_op_; }
    void                SetGizmosOp(ImGuizmo::OPERATION op) { gizmos_op_ = op; }

  private:
    std::shared_ptr<Scene> scene_;
    EditorLayer*           parent_;

    glm::vec2                    viewport_bounds_[2];
    bool                         viewport_focused_{false};
    bool                         viewport_hovered_{false};
    std::shared_ptr<FrameBuffer> frame_buffer_;
    Entity                       hovered_entity_{entt::null};
    EditorCamera                 editor_camera_;
    ImGuizmo::OPERATION          gizmos_op_{ImGuizmo::TRANSLATE};
    ImVec2                       viewport_size_;
};
}  // namespace samui

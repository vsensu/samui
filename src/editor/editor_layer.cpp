// clang-format off
#include "editor_layer.h"
// clang-format on

namespace samui {
bool DecomposeTransform(const glm::mat4& transform, glm::vec3& translation,
                        glm::vec3& rotation, glm::vec3& scale) {
  // From glm::decompose in matrix_decompose.inl

  using namespace glm;
  using T = float;

  mat4 LocalMatrix(transform);

  // Normalize the matrix.
  if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
    return false;

  // First, isolate perspective.  This is the messiest.
  if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
      epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
      epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())) {
    // Clear the perspective partition
    LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] =
        static_cast<T>(0);
    LocalMatrix[3][3] = static_cast<T>(1);
  }

  // Next take care of translation (easy).
  translation = vec3(LocalMatrix[3]);
  LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

  vec3 Row[3], Pdum3;

  // Now get scale and shear.
  for (length_t i = 0; i < 3; ++i)
    for (length_t j = 0; j < 3; ++j) Row[i][j] = LocalMatrix[i][j];

  // Compute X scale factor and normalize first row.
  scale.x = length(Row[0]);
  Row[0] = detail::scale(Row[0], static_cast<T>(1));
  scale.y = length(Row[1]);
  Row[1] = detail::scale(Row[1], static_cast<T>(1));
  scale.z = length(Row[2]);
  Row[2] = detail::scale(Row[2], static_cast<T>(1));

  // At this point, the matrix (in rows[]) is orthonormal.
  // Check for a coordinate system flip.  If the determinant
  // is -1, then negate the matrix and the scaling factors.
#if 0
		Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
		if (dot(Row[0], Pdum3) < 0)
		{
			for (length_t i = 0; i < 3; i++)
			{
				scale[i] *= static_cast<T>(-1);
				Row[i] *= static_cast<T>(-1);
			}
		}
#endif

  rotation.y = asin(-Row[0][2]);
  if (cos(rotation.y) != 0) {
    rotation.x = atan2(Row[1][2], Row[2][2]);
    rotation.z = atan2(Row[0][1], Row[0][0]);
  } else {
    rotation.x = atan2(-Row[2][0], Row[1][1]);
    rotation.z = 0;
  }

  return true;
}

EditorLayer::EditorLayer()
    : Layer("EditorLayer"), camera_controller_(1280.f / 720.f, true) {}

void EditorLayer::OnAttach() {
  SAMUI_PROFILE_FUNCTION();
  RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
  camera_controller_.SetZoomLevel(5.f);

  FrameBufferSpecification spec;
  spec.attachments = {
      FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RED_INTEGER,
      FrameBufferTextureFormat::RGBA, FrameBufferTextureFormat::RGBA,
      FrameBufferTextureFormat::Depth};
  spec.width = 1280;
  spec.height = 720;
  frame_buffer_ = FrameBuffer::Create(spec);

  active_scene_ = std::make_shared<Scene>();

  scene_hierarchy_panel_ = std::make_shared<SceneHierarchyPanel>(active_scene_);
  content_browser_ = std::make_shared<ContentBrowser>();

  editor_camera_ = EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);

  viewport_bounds_[0] = {0.f, 0.f};
  viewport_bounds_[1] = {0.f, 0.f};
}

void EditorLayer::OnDetach() { SAMUI_PROFILE_FUNCTION(); }

void EditorLayer::OnUpdate(const Timestep& deltaTime) {
  SAMUI_PROFILE_FUNCTION();

  if (viewport_focused_) {
    camera_controller_.OnUpdate(deltaTime);
  }

  editor_camera_.OnUpdate(deltaTime);

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
    frame_buffer_->ClearAttachment(1, -1);

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
    auto group = active_scene_->registry()
                     .group<TransformComponent, SpriteRendererComponent>();
    for (auto entity : group) {
      auto& transform = group.get<TransformComponent>(entity);
      auto& sprite = group.get<SpriteRendererComponent>(entity);
      Renderer2D::DrawSprite(transform.transform(), sprite, (int)entity);
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
  if (cameras.size_hint() > 0) {
    static int          camera_index = 0;
    auto                items = new const char*[cameras.size_hint()];
    std::vector<Entity> camera_entities;
    int                 index = 0;
    cameras.each(
        [&](const auto& camera, const auto& name, const auto& transform) {
          items[index++] = name.name.c_str();
          camera_entities.push_back(
              entt::to_entity(active_scene_->registry(), name));
        });

    ImGui::Combo("Main Camera", &camera_index, items, index);
    main_camera_ = camera_entities[camera_index];
    delete[] items;
  }

  static int color_attachment_index = 0;
  ImGui::DragInt("Color Attachment", &color_attachment_index, 1, 0,
                 frame_buffer_->ColorAttachmentCount() - 1);

  if (hovered_entity_ != entt::null) {
    ImGui::Text(
        "Hovered Entity: %s",
        active_scene_->GetComponent<NameComponent>(hovered_entity_).name);
  }

  ImGui::End();  // settings pannel end

  // scene pannel begin
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.f, 0.f});
  ImGui::Begin("Viewport");
  // auto viewport_offset = ImGui::GetCursorPos();

  viewport_focused_ = ImGui::IsWindowFocused();
  viewport_hovered_ = ImGui::IsWindowHovered();
  Application::Get().GetImGuiLayer()->BlockEvents(!viewport_focused_ &&
                                                  !viewport_hovered_);
  const auto& viewport_size = ImGui::GetContentRegionAvail();
  if (viewport_size.x > 0 && viewport_size.y > 0) {
    if (viewport_size.x != last_viewport_size_.x ||
        viewport_size.y != last_viewport_size_.y) {
      frame_buffer_->Resize(viewport_size.x, viewport_size.y);
      camera_controller_.OnResize(viewport_size.x, viewport_size.y);
      if (main_camera_ != entt::null) {
        auto& camera_comp =
            active_scene_->GetComponent<CameraComponent>(main_camera_);
        if (!camera_comp.fixed_aspect_ratio) {
          camera_comp.aspect_ratio = viewport_size.x / viewport_size.y;
        }
      }
      // main_camera_.OnResize();
      editor_camera_.SetViewportSize(viewport_size.x, viewport_size.y);
    }
  }
  last_viewport_size_ = viewport_size;
  uint32_t texture_id =
      frame_buffer_->GetColorAttachmentRenderID(color_attachment_index);
  // flip y
  ImGui::Image((ImTextureID)texture_id, viewport_size, {0.f, 1.f}, {1.f, 0.f});

  auto window_size = ImGui::GetWindowSize();
  auto min_bound = ImGui::GetWindowPos();
  // min_bound.x += viewport_offset.x;
  // min_bound.y += viewport_offset.y;

  ImVec2 max_bound = {min_bound.x + window_size.x, min_bound.y + window_size.y};
  viewport_bounds_[0] = {min_bound.x, min_bound.y};
  viewport_bounds_[1] = {max_bound.x, max_bound.y};

  // Draw Gizmos
  Entity selected_entity = scene_hierarchy_panel_->GetSelectedEntity();
  if (selected_entity != entt::null) {
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y,
                      ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

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
        active_scene_->GetComponent<TransformComponent>(selected_entity);
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
      DecomposeTransform(transform, translation, rotation, scale);
      glm::vec3 delta_rotation = rotation - trans_comp.rotation;
      trans_comp.translation = translation;
      trans_comp.rotation += delta_rotation;
      trans_comp.scale = scale;
    }
  }

  ImGui::End();
  ImGui::PopStyleVar();
  // scene pannel end

  // scene hierarchy pannel begin
  scene_hierarchy_panel_->OnImGuiRender();
  // scene hierarchy pannel end

  content_browser_->OnImGuiRender();
}

void EditorLayer::OnEvent(Event& event) {
  camera_controller_.OnEvent(event);
  editor_camera_.OnEvent(event);
  EventDispatcher dispatcher(event);
  dispatcher.Dispatch<KeyPressedEvent>(
      BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));
  dispatcher.Dispatch<MouseButtonPressedEvent>(
      BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));
}

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
  auto&    style = ImGui::GetStyle();
  float    min_size_x = style.WindowMinSize.x;
  style.WindowMinSize.x = 370.f;
  if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
    ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
  }
  style.WindowMinSize.x = min_size_x;

  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      // Disabling fullscreen would allow the window to be moved to the front of
      // other windows, which we can't undo at the moment without finer window
      // depth/z control.
      // ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);

      if (ImGui::MenuItem("New", "Ctrl+N")) {
        NewScene();
      }

      if (ImGui::MenuItem("Open", "Ctrl+O")) {
        LoadScene();
      }

      if (ImGui::MenuItem("Save", "Ctrl+Shift+S")) {
        SaveSceneAs();
      }

      if (ImGui::MenuItem("Exit")) {
        Application::Get().Close();
      }
      ImGui::EndMenu();
    }

    ImGui::EndMenuBar();
  }

  ImGui::End();
}

void EditorLayer::NewScene() {
  main_camera_ = entt::null;
  active_scene_ = std::make_shared<Scene>();
  scene_hierarchy_panel_->SetScene(active_scene_);
}

void EditorLayer::LoadScene() {
  auto filepath = DialogUtils::OpenFile("Samui Scene(*.samui)\0*.samui\0");
  if (!filepath.empty()) {
    main_camera_ = entt::null;
    active_scene_ = std::make_shared<Scene>();
    scene_hierarchy_panel_->SetScene(active_scene_);

    Serialization::DeserializeScene(*active_scene_, filepath);
  }
}

void EditorLayer::SaveSceneAs() {
  auto filepath = DialogUtils::SaveFile("Samui Scene(*.samui)\0*.samui\0");
  if (!filepath.empty()) {
    Serialization::SerializeScene(*active_scene_, filepath);
  }
}

bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
  if (event.GetRepeatCount() > 0) return false;

  bool ctrl_pressed = Input::IsKeyPressed(SAMUI_KEY_LEFT_CONTROL) ||
                      Input::IsKeyPressed(SAMUI_KEY_RIGHT_CONTROL);

  bool shift_pressed = Input::IsKeyPressed(SAMUI_KEY_LEFT_SHIFT) ||
                       Input::IsKeyPressed(SAMUI_KEY_RIGHT_SHIFT);

  switch (event.GetKeyCode()) {
    case SAMUI_KEY_N: {
      if (ctrl_pressed) {
        NewScene();
      }
    } break;
    case SAMUI_KEY_O: {
      if (ctrl_pressed) {
        LoadScene();
      }
    } break;
    case SAMUI_KEY_S: {
      if (ctrl_pressed && shift_pressed) {
        SaveSceneAs();
      }
    } break;
    case SAMUI_KEY_W: {
      gizmos_op_ = ImGuizmo::OPERATION::TRANSLATE;
    } break;
    case SAMUI_KEY_E: {
      gizmos_op_ = ImGuizmo::OPERATION::ROTATE;
    } break;
    case SAMUI_KEY_R: {
      gizmos_op_ = ImGuizmo::OPERATION::SCALE;
    } break;
    default:
      return false;
  }

  return true;
}

bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
  if (hovered_entity_ != entt::null && !ImGuizmo::IsOver() && !Input::IsKeyPressed(SAMUI_KEY_LEFT_CONTROL)) {
    scene_hierarchy_panel_->SetSelectedEntity(hovered_entity_);
    return false;
  }

  return true;
}

}  // namespace samui

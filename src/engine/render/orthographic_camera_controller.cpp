#include "orthographic_camera_controller.h"

#include "core/input.h"
#include "core/keycodes.h"
#include "debug/instrumentor.h"

namespace samui {
OrthographicCameraController::OrthographicCameraController(float aspect_ratio,
                                                           bool  rotation)
    : aspect_ratio_(aspect_ratio),
      rotation_enabled_(rotation),
      camera_(-aspect_ratio * zoom_level_, aspect_ratio * zoom_level_,
              -zoom_level_, zoom_level_) {}

void OrthographicCameraController::OnUpdate(const Timestep& deltaTime) {
  SAMUI_PROFILE_FUNCTION();
  auto camera_pos = camera_.get_position();
  if (Input::IsKeyPressed(SAMUI_KEY_A)) {
    camera_pos.x -= 1.f * deltaTime;
  }

  if (Input::IsKeyPressed(SAMUI_KEY_D)) {
    camera_pos.x += 1.f * deltaTime;
  }

  if (Input::IsKeyPressed(SAMUI_KEY_W)) {
    camera_pos.y += 1.f * deltaTime;
  }

  if (Input::IsKeyPressed(SAMUI_KEY_S)) {
    camera_pos.y -= 1.f * deltaTime;
  }

  if (rotation_enabled_) {
    auto rotation = camera_.get_rotation();
    if (Input::IsKeyPressed(SAMUI_KEY_Q)) {
      rotation += 10.f * deltaTime;
    }
    if (Input::IsKeyPressed(SAMUI_KEY_E)) {
      rotation -= 10.f * deltaTime;
    }
    camera_.set_rotation(rotation);
  }

  camera_.set_position(camera_pos);
}

void OrthographicCameraController::OnEvent(Event& e) {
  SAMUI_PROFILE_FUNCTION();
  EventDispatcher dispatcher(e);
  dispatcher.Dispatch<MouseScrolledEvent>(
      BIND_EVENT_FUNC(OrthographicCameraController::OnMouseScrolled));

  dispatcher.Dispatch<WindowResizeEvent>(
      BIND_EVENT_FUNC(OrthographicCameraController::OnWindowResized));
}

void OrthographicCameraController::OnResize(float width, float height) {
  aspect_ratio_ = width / height;
  CalculateView();
}

void OrthographicCameraController::CalculateView() {
  bounds_ = {-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
             -zoom_level_, zoom_level_};
  camera_.set_projection(bounds_.left, bounds_.right, bounds_.bottom,
                         bounds_.top);
}

bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
  SAMUI_PROFILE_FUNCTION();
  zoom_level_ -= e.GetYOffset() * 0.2f;
  zoom_level_ = std::max(zoom_level_, 0.25f);
  CalculateView();

  return false;
}

bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e) {
  SAMUI_PROFILE_FUNCTION();
  aspect_ratio_ = e.GetWidth() / (float)e.GetHeight();
  CalculateView();

  return false;
}
}  // namespace samui

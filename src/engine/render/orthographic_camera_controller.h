#ifndef SAMUI_ORTHOGRAPHIC_CAMERA_CONTROLLER_H_
#define SAMUI_ORTHOGRAPHIC_CAMERA_CONTROLLER_H_

#include "core/core.h"
#include "core/timestep.h"
#include "events/application_event.h"
#include "events/event.h"
#include "events/mouse_event.h"
#include "orthographic_camera.h"

namespace samui {
struct OrthographicCameraBounds {
  float left, right, bottom, top;

  float GetWidth() { return right - left; }
  float GetHeight() { return top - bottom; }
};

class SAMUI_API OrthographicCameraController {
 public:
  OrthographicCameraController(float aspect_ratio, bool rotation = false);

  void OnUpdate(const Timestep& deltaTime);
  void OnEvent(Event& e);

  void OnResize(float width, float height);

  const OrthographicCamera& GetCamera() { return camera_; }

  float GetZoomLevel() const { return zoom_level_; }
  void  SetZoomLevel(float zoom_level) {
    zoom_level_ = zoom_level;
    CalculateView();
  }

  const OrthographicCameraBounds& GetBounds() const { return bounds_; }

 private:
  void CalculateView();
  bool OnMouseScrolled(MouseScrolledEvent& e);
  bool OnWindowResized(WindowResizeEvent& e);

 private:
  float aspect_ratio_{16 / 9.f};
  float zoom_level_{1.f};
  bool  rotation_enabled_{false};

  OrthographicCamera       camera_;
  OrthographicCameraBounds bounds_;
};

}  // namespace samui

#endif  // SAMUI_ORTHOGRAPHIC_CAMERA_CONTROLLER_H_

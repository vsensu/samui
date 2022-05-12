#pragma once

// clang-format off
#include "core/core.h"
#include "core/timestep.h"
#include "events/application_event.h"
#include "events/event.h"
#include "events/mouse_event.h"
#include "orthographic_camera.h"
// clang-format on

namespace samui
{
struct OrthographicCameraBounds
{
    float left, right, bottom, top;

    float get_width() { return right - left; }
    float get_height() { return top - bottom; }
};

class SAMUI_API OrthographicCameraController
{
  public:
    OrthographicCameraController(float aspect_ratio, bool rotation = false);

    void on_update(const Timestep& deltaTime);
    void on_event(Event& e);

    void on_resize(float width, float height);

    const OrthographicCamera& get_camera() { return camera_; }

    float get_zoom_level() const { return zoom_level_; }
    void  set_zoom_level(float zoom_level)
    {
        zoom_level_ = zoom_level;
        calculate_view();
    }

    const OrthographicCameraBounds& get_bounds() const { return bounds_; }

  private:
    void calculate_view();
    bool on_mouse_scrolled(MouseScrolledEvent& e);
    bool on_window_resized(WindowResizeEvent& e);

  private:
    float aspect_ratio_{16 / 9.f};
    float zoom_level_{1.f};
    bool  rotation_enabled_{false};

    OrthographicCamera       camera_;
    OrthographicCameraBounds bounds_;
};

}  // namespace samui

#include <samui.h>

#include <iostream>
// #if defined(_MSC_VER) && (_MSC_VER >= 1900) && \
//     !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
// #pragma comment(lib, "legacy_stdio_definitions")
// #endif

class ExampleLayer : public samui::Layer {
 public:
  ExampleLayer() : Layer("Example") {}
  void OnUpdate() override {
    if (samui::Input::IsKeyPressed(SAMUI_KEY_TAB)) {
      SAMUI_TRACE("Tab is pressed!");
    }
  }
  void OnEvent(samui::Event& event) override {
    if (event.GetEventType() == samui::EventType::KeyPressed) {
      auto &key_press_event = static_cast<samui::KeyPressedEvent&>(event);
      SAMUI_TRACE("Input:{0}", (char)key_press_event.GetKeyCode());
    }
  }
};

class Demo : public samui::Application {
 public:
  Demo() : Application() {
    PushLayer(new ExampleLayer());
    PushLayer(new samui::ImGuiLayer());
  }
};

samui::Application* samui::create_application() {
  SAMUI_ENGINE_TRACE("engine trace");
  SAMUI_ENGINE_INFO("engine info");
  SAMUI_ENGINE_WARN("engine warn");
  SAMUI_ENGINE_ERROR("engine error");
  SAMUI_ENGINE_FATAL("engine fatal");

  SAMUI_TRACE("game trace");
  SAMUI_INFO("game info");
  SAMUI_WARN("game warn");
  SAMUI_ERROR("game error");
  SAMUI_FATAL("game fatal");

  return new Demo();
}

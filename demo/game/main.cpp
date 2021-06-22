#include <imgui.h>
#include <samui.h>

#include <iostream>

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
      auto& key_press_event = static_cast<samui::KeyPressedEvent&>(event);
      SAMUI_TRACE("Input:{0}", (char)key_press_event.GetKeyCode());
    }
  }

  void OnImGuiRender() override {
    static bool show = true;
    ImGui::ShowDemoWindow(&show);
  }
};

class Demo : public samui::Application {
 public:
  Demo() : Application() {
    PushLayer(new ExampleLayer());
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

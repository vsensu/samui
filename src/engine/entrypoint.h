#pragma once

#ifdef SAMUI_PLATFORM_WINDOWS

extern samui::Application* samui::create_application();

int main(int argc, char** argv) {
  samui::Log::Init();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Startup.json");
  auto* app = samui::create_application();
  SAMUI_PROFILE_END_SESSION();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Runtime.json");
  app->run();
  SAMUI_PROFILE_END_SESSION();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Shutdown.json");
  delete app;
  app = nullptr;
  SAMUI_PROFILE_END_SESSION();

  return 0;
}

#else
#error samui only support windows!
#endif

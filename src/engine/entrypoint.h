#pragma once

#ifdef SAMUI_PLATFORM_WINDOWS

// clang-format off
#include <memory>

#include <profiler/instrumentor.h>
#include <core/application.h> 
#include <core/engine.h>
// clang-format on


// extern samui::Application* samui::create_application();
// samui::Application* samui::create_application();
namespace samui
{
std::shared_ptr<Application> create_application();
}

int main(int argc, char** argv) {
  samui::Log::Init();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Startup.json");
  samui::Engine& engine = samui::Engine::instance();
  std::shared_ptr<samui::Application> app = samui::create_application();
  engine.set_active_app(app)
  SAMUI_PROFILE_END_SESSION();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Runtime.json");
  app->run();
  SAMUI_PROFILE_END_SESSION();

  SAMUI_PROFILE_BEGIN_SESSION("Startup", "SamuiProfile-Shutdown.json");
  app.reset();
  SAMUI_PROFILE_END_SESSION();

  return 0;
}

#else
#error samui only support windows!
#endif

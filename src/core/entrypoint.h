#ifndef SAMUI_ENTRYPOINT_H_
#define SAMUI_ENTRYPOINT_H_

#ifdef SAMUI_PLATFORM_WINDOWS

extern samui::Application* samui::create_application();
// extern void entry();

int main(int argc, char** argv) {
  samui::Log::Init();
  auto* app = samui::create_application();
  app->Run();
  delete app;
  app = nullptr;
  // entry();
  return 0;
}

#else
#error samui only support windows!
#endif

#endif
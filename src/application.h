#ifndef SAMUI_APPLICATION_H_
#define SAMUI_APPLICATION_H_

#include "Window.h"
#include "core.h"

namespace samui {

class SAMUI_API Application {
 private:
  /* data */
 public:
  Application(/* args */);
  virtual ~Application();
  virtual void Run();

 private:
  std::unique_ptr<Window> window_;
  bool                    running_{true};
};

Application* create_application();

}  // namespace samui

#endif
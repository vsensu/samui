#ifndef SAMUI_APPLICATION_H_
#define SAMUI_APPLICATION_H_

#include "core.h"

namespace samui {

class SAMUI_API Application {
 private:
  /* data */
 public:
  Application(/* args */);
  virtual ~Application();
  virtual void Run();
};

Application* create_application();

}  // namespace samui

#endif
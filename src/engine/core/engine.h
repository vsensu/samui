#pragma once

// clang-format off
#include <memory>

#include "_inc.h"
#include "application.h"
#include "singleton.h"
// clang-format on

namespace samui
{
class SAMUI_CORE_API Engine : public Singleton<Engine>
{
  public:
    std::shared_ptr<Application> app() { return app_; }

  private:
    std::shared_ptr<Application> app_;
};
}  // namespace samui

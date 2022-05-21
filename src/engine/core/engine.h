#pragma once

// clang-format off
#include <memory>

#include "_inc.h"
#include "application.h"
// clang-format on

namespace samui
{
class SAMUI_CORE_API Engine
{
  public:
    static Engine& instance();
    
    void set_active_app(std::shared_ptr<Application> app);
    std::shared_ptr<Application> app();


  private:
    std::shared_ptr<Application> app_ {nullptr};
};
}  // namespace samui

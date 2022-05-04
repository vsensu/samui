#pragma once

#include "core.h"

namespace samui
{
class SAMUI_API Timestep
{
  public:
    Timestep(float timeInSeconds = 0.f) : time_in_seconds_(timeInSeconds) {}

          operator float() const { return time_in_seconds_; }
    float time_in_seconds() const { return time_in_seconds_; }
    float time_in_milliseconds() const { return time_in_seconds_ * 1000.f; }

  private:
    float time_in_seconds_{0.f};
};
}  // namespace samui

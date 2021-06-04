#ifndef SAMUI_LOG_H_H
#define SAMUI_LOG_H_H

#include <spdlog/spdlog.h>

#include <memory>

#include "core.h"

namespace samui {

class SAMUI_API Log {
 public:
  static void Init();

  static inline std::shared_ptr<spdlog::logger>& GetEngineLogger();
  static inline std::shared_ptr<spdlog::logger>& GetGameLogger();

 private:
  static std::shared_ptr<spdlog::logger> engine_logger_;
  static std::shared_ptr<spdlog::logger> game_logger_;
};

}  // namespace samui

// clang-format off
#define SAMUI_ENGINE_TRACE(...)     samui::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define SAMUI_ENGINE_INFO(...)      samui::Log::GetEngineLogger()->info(__VA_ARGS__)
#define SAMUI_ENGINE_WARN(...)      samui::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define SAMUI_ENGINE_ERROR(...)     samui::Log::GetEngineLogger()->error(__VA_ARGS__)
#define SAMUI_ENGINE_FATAL(...)     samui::Log::GetEngineLogger()->critical(__VA_ARGS__)

#define SAMUI_TRACE(...)     samui::Log::GetGameLogger()->trace(__VA_ARGS__)
#define SAMUI_INFO(...)      samui::Log::GetGameLogger()->info(__VA_ARGS__)
#define SAMUI_WARN(...)      samui::Log::GetGameLogger()->warn(__VA_ARGS__)
#define SAMUI_ERROR(...)     samui::Log::GetGameLogger()->error(__VA_ARGS__)
#define SAMUI_FATAL(...)     samui::Log::GetGameLogger()->critical(__VA_ARGS__)
// clang-format on

#endif

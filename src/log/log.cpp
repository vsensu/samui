#include "log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace samui {
std::shared_ptr<spdlog::logger> Log::engine_logger_;
std::shared_ptr<spdlog::logger> Log::game_logger_;

void Log::Init() {
  spdlog::set_pattern("%^[%T] %n: %v%$");
  engine_logger_ = spdlog::stdout_color_mt("SAMUI");
  engine_logger_->set_level(spdlog::level::trace);

  game_logger_ = spdlog::stdout_color_mt("GAME");
  game_logger_->set_level(spdlog::level::trace);

  SAMUI_ENGINE_INFO("Initialized Log!");
}

std::shared_ptr<spdlog::logger>& Log::GetEngineLogger() {
  return engine_logger_;
}

std::shared_ptr<spdlog::logger>& Log::GetGameLogger() { return game_logger_; }

}  // namespace samui

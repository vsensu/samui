#include "engine.h"

namespace samui
{

Engine& Engine::instance()
{
    // This is a thread-safe singleton.
    // c++ compiler will confirm the initialization of the local static
    // variable only once.
    static Engine* instance = new Engine();

    return *instance;
}

void Engine::set_active_app(std::shared_ptr<Application> app)
{
    if (app_ == app) return;

    if (app_ != nullptr) app_->close();

    app_ = app;
    if (app != nullptr) app->init();
}

std::shared_ptr<Application> Engine::app() { return app_; }
}  // namespace samui

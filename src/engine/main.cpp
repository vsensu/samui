#include "common/Application.h"
#include <entt/entt.hpp>

int game_loop(entt::registry &registry)
{
    Application app;
    return app.Run();
}

int main(int argc, char **argv)
{
    entt::registry registry;
    return game_loop(registry);
}

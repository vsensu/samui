#pragma once

// clang-format off
#include <string>

#include <entt/entt.hpp>

#include <log/log.h>
#include <assert/assert.h>

#include "scene_module.h"
#include "components.h"
// clang-format on

namespace samui
{
using Entity = entt::entity;
class SAMUI_SCENE_API Scene
{
public:
    Scene();

    Entity create_entity(const std::string& name = "Entity")
    {
        auto entity = registry_.create();
        add_component<NameComponent>(entity, name);
        add_component<TransformComponent>(entity);
        return entity;
    }

    void destroy_entity(Entity entity);

    template <typename T, typename... Args>
    T& add_component(Entity entity, Args&&... args)
    {
        SAMUI_ENGINE_ASSERT(!has_component<T>(entity),
                            "entity already has component!");
        return registry_.emplace<T>(entity, std::forward<Args>(args)...);
    }

    template <typename T>
    T& get_component(Entity entity)
    {
        SAMUI_ENGINE_ASSERT(has_component<T>(entity),
                            "entity does not has component!");
        return registry_.get<T>(entity);
    }

    template <typename T>
    bool has_component(Entity entity) const
    {
        return registry_.any_of<T>(entity);
    }

    template <typename T>
    void remove_component(Entity entity)
    {
        SAMUI_ENGINE_ASSERT(HasComponent<T>(entity),
                            "entity does not has component!");
        registry_.remove<T>(entity);
    }

    entt::registry& registry() { return registry_; }

private:
    entt::registry registry_;
};

}  // namespace samui

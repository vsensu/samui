#include "collision.h"

namespace samui
{
CollisionResponseType CollisionManager::test_collision(
    const CollisionSettings& a, const CollisionSettings& b)
{
    if (a.enabled && b.enabled)
    {
        auto a_to_b =
            a.object_responses[static_cast<std::size_t>(b.object_type)];
        auto b_to_a =
            b.object_responses[static_cast<std::size_t>(a.object_type)];
        return response_map_[static_cast<std::size_t>(a_to_b)]
                            [static_cast<std::size_t>(b_to_a)];
    }

    return CollisionResponseType::Ignore;
}
}  // namespace samui

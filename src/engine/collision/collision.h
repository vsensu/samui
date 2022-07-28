#pragma once

/// ref: https://www.unrealengine.com/en-US/blog/collision-filtering

// clang-format off
#include <array>

#include <basic/types.h>

#include "collision_module.h"
// clang-format on

namespace samui
{

constexpr int CollisionObjectTypeMax = 32;

enum class SAMUI_COLLISION_API CollisionObjectType : u8
{
    Static = 0,
    Dynamic,
    Pawn,
    Player,

    CustomStart,
};

enum class SAMUI_COLLISION_API CollisionResponseType : u8
{
    Ignore = 0,
    Overlap = 1,
    Block = 2,
};

struct SAMUI_COLLISION_API CollisionSettings
{
    bool                                  enabled;
    CollisionObjectType                   object_type;
    CollisionResponseType                 trace_response;
    std::array<CollisionResponseType, 32> object_responses{};
};

template <size_t N, class T>
constexpr std::array<T, N> SAMUI_COLLISION_API make_array(const T& v)
{
    std::array<T, N> ret;
    ret.fill(v);
    return ret;
}

struct SAMUI_COLLISION_API CollisionPresets
{
    static constexpr CollisionSettings NoCollision{
        .enabled = true,
        .object_type = CollisionObjectType::Static,
        .trace_response = CollisionResponseType::Ignore,
        .object_responses =
            make_array<CollisionObjectTypeMax>(CollisionResponseType::Ignore),
    };

    static constexpr CollisionSettings BlockAll{
        .enabled = true,
        .object_type = CollisionObjectType::Static,
        .trace_response = CollisionResponseType::Block,
        .object_responses =
            make_array<CollisionObjectTypeMax>(CollisionResponseType::Block),
    };

    static constexpr CollisionSettings Pawn{
        .enabled = true,
        .object_type = CollisionObjectType::Pawn,
        .trace_response = CollisionResponseType::Overlap,
        .object_responses =
            make_array<CollisionObjectTypeMax>(CollisionResponseType::Block),
    };
};

class SAMUI_COLLISION_API CollisionManager
{
public:
    static CollisionResponseType test_collision(const CollisionSettings& a,
                                                const CollisionSettings& b);

private:
    static constexpr CollisionResponseType response_map_[3][3]{
        {CollisionResponseType::Ignore, CollisionResponseType::Ignore,
         CollisionResponseType::Ignore},
        {CollisionResponseType::Ignore, CollisionResponseType::Overlap,
         CollisionResponseType::Overlap},
        {CollisionResponseType::Ignore, CollisionResponseType::Overlap,
         CollisionResponseType::Block}};
};

}  // namespace samui
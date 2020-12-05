#pragma once

#include <engine/network/net_protocol.h>

enum class ActionType : uint16_t
{
    Login,
    BroadCastUserMessage,
};

namespace samui::net
{
    template <>
    struct ResponseData<ActionType::Login>
    {
        uint64_t uid;
    };

    template <>
    struct RequestData<ActionType::BroadCastUserMessage>
    {
        char content[1024];
    };

    template <>
    struct ResponseData<ActionType::BroadCastUserMessage>
    {
        uint64_t uid;
        char user_name[16];
        char content[1024];
    };
} // namespace samui::net
#pragma once

#include <engine/network/net_protocol.h>

enum class ActionType : uint16_t
{
    Login,
    BroadCastUserMessage,
};

DEFINE_ACTION_TYPE(ActionType);

BEGIN_REQ_DATA(ActionType::BroadCastUserMessage)
    char content[1024];
END_REQ_DATA

BEGIN_RSP_DATA(ActionType::BroadCastUserMessage)
    uint64_t uid;
    char user_name[16];
    char content[1024];
END_REQ_DATA

BEGIN_RSP_DATA(ActionType::Login)
    uint64_t uid;
END_REQ_DATA

// template <>
// struct RspData<ActionType::Login>
// {
//     uint64_t uid;
// };

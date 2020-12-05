#pragma once

#include <string>

namespace samui::net
{
    template <auto action>
    struct RequestData
    {
    };

    template <auto action>
    struct ResponseData
    {
    };

    template <auto action>
    struct Request
    {
        constexpr static auto id = action;
        RequestData<action> data;
    };

    template <auto action>
    struct Response
    {
        constexpr static auto id = action;
        ResponseData<action> data;
    };

    template <typename T, typename U>
    void send(T *peer, const U &data)
    {
        Packet packet;
        packet << data.id; /* id is constexpr static, so must send by yourself */
        packet << data.data;
        enet_send_packet(peer, packet, 0, 0);
    }
} // namespace samui::net

#define BEGIN_REQ_DATA(action)          \
namespace samui::net {                  \
template <>                             \
struct RequestData<action>                  \
{                                       

#define END_REQ_DATA    };}

#define BEGIN_RSP_DATA(action)          \
namespace samui::net {                  \
template <>                             \
struct ResponseData<action>                  \
{                                       

#define END_RSP_DATA    };}

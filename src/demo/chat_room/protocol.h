#pragma once

#include <iostream>

#include "msg_type.h"

template <MessageType msg>
struct ReqData
{
};

template <MessageType msg>
struct RspData
{
};

template <MessageType msg>
struct Request
{
    constexpr static MessageType id = msg;
    ReqData<msg> data;
};

template <MessageType msg>
struct Response
{
    constexpr static MessageType id = msg;
    RspData<msg> data;
};

template <>
struct ReqData<MessageType::BroadCastUserMessage>
{
    char content[1024];
};

template <>
struct RspData<MessageType::BroadCastUserMessage>
{
    uint64_t uid;
    char user_name[16];
    char content[1024];
};

template <>
struct RspData<MessageType::ServerAccept>
{
    uint64_t uid;
};

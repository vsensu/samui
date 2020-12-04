#pragma once

#define DEFINE_ACTION_TYPE(Action)      \
namespace samui::net {                  \
template <Action msg>                   \
struct ReqData                          \
{                                       \
};                                      \
template <Action msg>                   \
struct RspData                          \
{                                       \
};                                      \
template <Action msg>                   \
struct Request                          \
{                                       \
    constexpr static Action id = msg;   \
    ReqData<msg> data;                  \
};                                      \
template <Action msg>                   \
struct Response                         \
{                                       \
    constexpr static Action id = msg;   \
    RspData<msg> data;                  \
};}                                      

#define BEGIN_REQ_DATA(action)          \
namespace samui::net {                  \
template <>                             \
struct ReqData<action>                  \
{                                       

#define END_REQ_DATA    };}

#define BEGIN_RSP_DATA(action)          \
namespace samui::net {                  \
template <>                             \
struct RspData<action>                  \
{                                       

#define END_RSP_DATA    };}

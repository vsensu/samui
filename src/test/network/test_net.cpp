#include <iostream>
#include <engine/network/net.h>

enum class CustomMsgTypes : uint32_t
{
    FireBullet,
    MovePlayer
};

class CustomClient : public samui::net::client_interface<CustomMsgTypes>
{
public:
    bool FireBullet(float x, float y)
    {
        samui::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::FireBullet;
        msg << x << y;
        send(msg);
    }
};

int main()
{
    CustomClient c;
    c.connect("community.onelonecoder.com", 60000);
    c.FireBullet(2.0f, 5.0f);

    return 0;
}

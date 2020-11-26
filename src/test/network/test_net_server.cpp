#include <iostream>
#include <engine/network/net.h>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomServer : public samui::net::server_interface<CustomMsgTypes>
{
public:
    CustomServer(uint16_t port) : samui::net::server_interface<CustomMsgTypes>(port)
    {
    }

protected:
    bool on_client_connect (std::shared_ptr<samui::net::connection<CustomMsgTypes>> client) override
    {
        samui::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->send(msg);
        return true;
    }

    void on_client_disconnect (std::shared_ptr<samui::net::connection<CustomMsgTypes>> client) override
    {
        std::cout << "Removing client [" << client->GetID() << "]\n";
    }

    void on_message (std::shared_ptr<samui::net::connection<CustomMsgTypes>> client, samui::net::message<CustomMsgTypes> &msg) override
    {
        switch (msg.header.id)
        {
        case CustomMsgTypes::ServerPing:
        {
            std::cout << "[" << client->GetID() << "]: Server Ping\n";
            client->send(msg);
        }
            break;
        }
    }
};

int main()
{
    CustomServer server(60000);
    server.start();

    while(1)
    {
        server.update();
    }
    return 0;
}

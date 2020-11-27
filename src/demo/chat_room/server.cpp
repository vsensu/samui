#include <iostream>
#include <string>

#include <engine/network/net.h>

#include "msg_type.h"

using ParentServerType = samui::net::server_interface<Message>;
class ChatRoomServer : public ParentServerType
{
public:
    using ParentServerType::ParentServerType;
protected:
    bool on_client_connect (std::shared_ptr<samui::net::connection<Message>> client) override
    {
        samui::net::message<Message> msg;
        msg.header.id = Message::ServerAccept;
        client->send(msg);
        return true;
    }

    void on_client_disconnect (std::shared_ptr<samui::net::connection<Message>> client) override
    {
        std::cout << "Removing client [" << client->GetID() << "]\n";
    }

    void on_message (std::shared_ptr<samui::net::connection<Message>> client, samui::net::message<Message> &msg) override
    {
        switch (msg.header.id)
        {
        case Message::BroadCastUserMessage:
        {
            // std::vector<char> bytes(msg.header.size);
            int bytes;
            msg >> bytes;
            // std::string text(bytes.begin(), bytes.end());
            // std::cout << "[" << client->GetID() << "]<BroadCastUserMessage>: " << text << "\n";
            std::cout << "[" << client->GetID() << "]<BroadCastUserMessage>: " << bytes << "\n";
            message_all_clients(msg, client);
        }
            break;
        }
    }
};

int main()
{
    ChatRoomServer server(60000);
    server.start();

    while(1)
    {
        server.update(-1, true);
    }
    return 0;
}

#include <iostream>
#include <string>

#include <engine/common/Application.h>
#include <engine/network/net.h>
#include <imgui.h>

#include "protocol.h"

class ChatRoomClient : public Application
{
public:
    using Application::Application;
    int connect()
    {
        client = enet_host_create(NULL /* create a client host */,
                                  1 /* only allow 1 outgoing connection */,
                                  1 /* allow up 2 channels to be used, 0 and 1 */,
                                  0 /* assume any amount of incoming bandwidth */,
                                  0 /* assume any amount of outgoing bandwidth */);
        if (nullptr == client)
        {
            std::cerr << "An error occurred while trying to create an ENet client host.\n";
            exit(EXIT_FAILURE);
        }

        ENetAddress address;
        enet_address_set_host(&address, "127.0.0.1");
        address.port = 7777;

        peer = enet_host_connect(client, &address, 1, 0);
        if (nullptr == peer)
        {
            std::cerr << "No available peers for initiating an ENet connection\n";
            return EXIT_FAILURE;
        }

        // Wait for a connection establishment
        bool connected = [this] {
            ENetEvent event;
            while (enet_host_service(client, &event, 5000) > 0)
            {
                if (event.type == ENET_EVENT_TYPE_RECEIVE)
                {
                    enet_packet_destroy(event.packet);
                }
                else if (event.type == ENET_EVENT_TYPE_CONNECT)
                {
                    return true;
                }
            }
            return false;
        }();
        if (!connected)
        {
            std::cerr << "Failed to establish connection with the server.\n";
            return EXIT_FAILURE;
        }
    }

private:
    void RenderUI() override
    {
        ImGui::Begin("Chat Root");
        // ImGui::ShowDemoWindow();

        ImGui::Text(std::to_string(uid).c_str());
        if (enet_host_service(client, &event, 0) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::cout << "A packet of length "
                          << event.packet->dataLength
                          << " containing "
                          << event.packet->data
                          << " was received from "
                          << event.peer->address.host
                          << ":"
                          << event.peer->address.port
                          << " on channel "
                          << event.channelID
                          << ".\n";

                samui::net::Packet packet(*event.packet);
                MessageType msg_type;
                packet >> msg_type;
                switch (msg_type)
                {
                case MessageType::ServerAccept:
                {
                    RspData<MessageType::ServerAccept> data;
                    packet >> data;
                    uid = data.uid;
                }
                break;
                case MessageType::BroadCastUserMessage:
                {
                    RspData<MessageType::BroadCastUserMessage> data;
                    packet >> data;
                    content_recv += data.content;
                    content_recv += "\n";
                }
                break;
                }

                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);
            }
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
            {
                std::cout << event.peer->address.host << ":" << event.peer->address.port << " disconnected.\n";
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
            break;
            }
        }

        ImGui::Text(content_recv.c_str());

        static char text[1024 * 16] = "";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        ImGui::Text(text);
        if (ImGui::Button("Send"))
        {
            samui::net::Packet packet;
            Request<MessageType::BroadCastUserMessage> req;
            packet << req.id;
            std::memcpy(req.data.content, text, 1024);
            packet << req.data;
            samui::net::enet_send_packet(peer, packet, 0, 0);
            std::memset(text, 0, 1024);
        }
        ImGui::End();
    }

private:
    std::vector<char> content_to_send;
    std::string content_recv;

    ENetHost *client;
    ENetPeer *peer;
    ENetEvent event;
    uint64_t uid;
};

int main()
{
    samui::net::init_enet();

    ChatRoomClient client;
    client.connect();

    return client.Run();
}

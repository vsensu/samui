#include <iostream>
#include <string>

#include <engine/common/Application.h>
#include <engine/network/net.h>

#include "protocol.h"

int main()
{
    samui::net::init_enet();

    ENetHost *client = enet_host_create(NULL /* create a client host */,
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

    ENetPeer *peer = enet_host_connect(client, &address, 1, 0);
    if (nullptr == peer)
    {
        std::cerr << "No available peers for initiating an ENet connection\n";
        return EXIT_FAILURE;
    }

    // Wait for a connection establishment
    bool connected = [&client] {
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

    samui::net::Packet packet;
    Request<MessageType::BroadCastUserMessage> req;
    packet << req.id;
    std::string context = "hello world";
    std::memcpy(req.data.content, context.data(), context.size()+1);
    packet << req.data;
    samui::net::enet_send_packet(peer, packet, 0, 0);

    ENetEvent event;
    while (true)
    {
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
                // sf::Packet packet;
                // packet.append(event.packet->data, event.packet->dataLength);
                // std::string text;
                // packet >> text;
                // std::cout << text << "\n";
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
    }
    return 0;
}

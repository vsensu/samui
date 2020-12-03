#include <iostream>
#include <string>

#include <engine/network/net.h>

#include "protocol.h"

int main()
{
    samui::net::init_enet();

    ENetAddress address;
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 7777. */
    address.port = 7777;
    ENetHost *server = enet_host_create(&address /* the address to bind the server host to */,
                                        32 /* allow up to 32 clients and/or outgoing connections */,
                                        1 /* allow up to 2 channels to be used, 0 and 1 */,
                                        0 /* assume any amount of incoming bandwidth */,
                                        0 /* assume any amount of outgoing bandwidth */);
    if (nullptr == server)
    {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    while (true)
    {
        if (enet_host_service(server, &event, 0) > 0)
        {

            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << "\n";
                sf::Packet packet;
                packet << "hello";
                samui::net::enet_send_sf_packet(event.peer, packet, 0, 0);
            }
            break;
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
                case MessageType::BroadCastUserMessage:
                {
                    std::cout << "broadcast:";
                    ReqData<MessageType::BroadCastUserMessage> data;
                    packet >> data;
                    std::cout << data.content << "\n";
                }
                break;
                
                default:
                    std::cout << (int)msg_type << "\n";
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
    }
    return 0;
}

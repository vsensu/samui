#include <iostream>
#include <string>
#include <map>

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

    // std::map<uint64_t, std::tuple<uint32_t, uint16_t>> uid_address;
    std::map<std::tuple<uint32_t, uint16_t>, ENetPeer*> address_peer;

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
                samui::net::Response<ActionType::Login> rsp;
                static uint64_t uid = 0;
                uid += 1;

                address_peer[std::make_tuple(event.peer->address.host, event.peer->address.port)] = event.peer;

                rsp.data.uid = uid;
                samui::net::send(event.peer, rsp);
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
                ActionType msg_type;
                packet >> msg_type;
                switch (msg_type)
                {
                case ActionType::BroadCastUserMessage:
                {
                    std::cout << "broadcast:";
                    samui::net::Request<ActionType::BroadCastUserMessage> req;
                    packet >> req.data;

                    std::cout << req.data.content << "\n";

                    for (const auto &peer : address_peer)
                    {
                        if (peer.second != nullptr && !(peer.second->address.host == event.peer->address.host && peer.second->address.port == event.peer->address.port))
                        {
                            samui::net::Response<ActionType::BroadCastUserMessage> rsp;
                            rsp.data.uid = peer.second->address.port;
                            std::memcpy(rsp.data.content, req.data.content, 1024);
                            samui::net::send(peer.second, rsp);
                        }
                    }
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
                address_peer[std::make_tuple(event.peer->address.host, event.peer->address.port)] = nullptr;
                event.peer->data = NULL;
            }
            break;
            }
        }
    }
    return 0;
}

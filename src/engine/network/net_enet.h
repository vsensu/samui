#pragma once

#include <functional>

#include <enet/enet.h>
#include <SFML/Network/Packet.hpp>

namespace samui
{
    namespace net
    {
        int init_net()
        {
            if (enet_initialize() != 0)
            {
                std::cerr << "An error occurred while initializing ENet.\n";
                return EXIT_FAILURE;
            }
            atexit(enet_deinitialize);
        }

        ENetPacket *sf_to_enet_packet(const sf::Packet &packet, uint32_t flags)
        {
            return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
        }

        void send(ENetPeer *peer, const sf::Packet &packet, int channel, uint32_t flags)
        {
            auto enet_packet = sf_to_enet_packet(packet, flags);
            enet_peer_send(peer, channel, enet_packet);
        }

        sf::Packet enet_to_sf_packet(ENetPacket &enet_packet)
        {
            sf::Packet packet;
            packet.append(enet_packet.data, enet_packet.dataLength);
            return packet;
        }
    } // namespace net
} // namespace samui

#pragma once

#include <type_traits>

#include <enet/enet.h>
#include <SFML/Network/Packet.hpp>

namespace samui
{
    namespace net
    {
        template <typename T>
        concept trivially_copyable_t = std::is_trivially_copyable_v<T>;

        class Packet final
        {
        private:
            std::vector<uint8_t> body;
            std::size_t read_pos {0};   // Current reading position in the packet

        public:
            Packet() = default;
            Packet(ENetPacket &enet_packet)
            {
                body.resize(enet_packet.dataLength);
                std::memcpy(body.data(), enet_packet.data, enet_packet.dataLength);
            }

            size_t size() const
            {
                return body.size();
            }

            auto data() const
            {
                return body.data();
            }

            friend std::ostream& operator << (std::ostream &os, const Packet &msg)
            {
                os <<" Size:" << msg.size();
                return os;
            }

            template<trivially_copyable_t trivially_copyable>
            friend Packet& operator << (Packet &msg, const trivially_copyable &data)
            {
                size_t i = msg.body.size();
                msg.body.resize(msg.body.size() + sizeof(trivially_copyable));
                std::memcpy(msg.body.data() + i, &data, sizeof(trivially_copyable));
                return msg;
            }

            template<trivially_copyable_t trivially_copyable>
            friend Packet& operator >> (Packet &msg, trivially_copyable &data)
            {
                std::memcpy(&data, msg.body.data() + msg.read_pos, sizeof(trivially_copyable));
                msg.read_pos += sizeof(trivially_copyable);
                return msg;
            }
        };

        ENetPacket* to_enet_packet(const Packet &packet, uint32_t flags)
        {
            return enet_packet_create(packet.data(), packet.size(), flags);
        }

        void enet_send_packet(ENetPeer *peer, const Packet &packet, int channel, uint32_t flags)
        {
            auto enet_packet = to_enet_packet(packet, flags);
            enet_peer_send(peer, channel, enet_packet);
        }

        ENetPacket* sf_to_enet_packet(const sf::Packet &packet, uint32_t flags)
        {
            return enet_packet_create(packet.getData(), packet.getDataSize(), flags);
        }

        void enet_send_sf_packet(ENetPeer *peer, const sf::Packet &packet, int channel, uint32_t flags)
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

        int init_enet()
        {
            if (enet_initialize() != 0)
            {
                std::cerr << "An error occurred while initializing ENet.\n";
                return EXIT_FAILURE;
            }
            atexit(enet_deinitialize);
        }
    } // namespace net
} // namespace samui

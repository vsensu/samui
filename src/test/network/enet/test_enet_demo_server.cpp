#include <iostream>
#include <enet/enet.h>

void as_server()
{
    ENetAddress address;
    ENetHost *server;
    /* Bind the server to the default localhost.     */
    /* A specific host address can be specified by   */
    /* enet_address_set_host (& address, "x.x.x.x"); */
    address.host = ENET_HOST_ANY;
    /* Bind the server to port 7777. */
    address.port = 7777;
    server = enet_host_create(&address /* the address to bind the server host to */,
                              32 /* allow up to 32 clients and/or outgoing connections */,
                              1 /* allow up to 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (server == NULL)
    {
        std::cerr << "An error occurred while trying to create an ENet server host.\n";
        exit(EXIT_FAILURE);
    }

    ENetEvent event;
    while (true)
    {
        while (enet_host_service(server, &event, 10000) > 0)
        {
            switch (event.type)
            {
            case ENET_EVENT_TYPE_CONNECT:
            {
                std::cout << "A new client connected from " << event.peer->address.host << ":" << event.peer->address.port << "\n";
            }
            break;
            case ENET_EVENT_TYPE_RECEIVE:
            {
                std::cout << "A packet of length "
                          << event.packet->dataLength
                          << " containing "
                          << event.packet->data
                          << " was received from "
                          //   << event.peer->data
                          << event.peer->address.host
                          << ":"
                          << event.peer->address.port
                          << " on channel "
                          << event.channelID
                          << ".\n";
                /* Clean up the packet now that we're done using it. */
                enet_packet_destroy(event.packet);
            }
            break;
            case ENET_EVENT_TYPE_DISCONNECT:
            {

                // printf("%s disconnected.\n", event.peer->data);
                std::cout << event.peer->address.host << ":" << event.peer->address.port << " disconnected.\n";
                /* Reset the peer's client information. */
                event.peer->data = NULL;
            }
            break;
            }
        }
    }

    enet_host_destroy(server);
}

int main(int argc, char **argv)
{
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    as_server();
}

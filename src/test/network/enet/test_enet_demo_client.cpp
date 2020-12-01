#include <iostream>
#include <enet/enet.h>

int as_client()
{
    ENetHost *client;
    client = enet_host_create(NULL /* create a client host */,
                              1 /* only allow 1 outgoing connection */,
                              1 /* allow up 2 channels to be used, 0 and 1 */,
                              0 /* assume any amount of incoming bandwidth */,
                              0 /* assume any amount of outgoing bandwidth */);
    if (client == NULL)
    {
        std::cerr << "An error occurred while trying to create an ENet client host.\n";
        exit(EXIT_FAILURE);
    }

    ENetAddress address;
    ENetEvent event;
    ENetPeer *peer;

    enet_address_set_host(&address, "127.0.0.1");
    address.port = 7777;

    peer = enet_host_connect(client, &address, 1, 0);
    if (peer == NULL)
    {
        std::cerr << "No available peers for initiating an ENet connection\n";
        return EXIT_FAILURE;
    }

    if (enet_host_service(client, &event, 10000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connection to " << event.peer->address.host << ":" << event.peer->address.port << " succeed.\n";
    }
    else
    {
        enet_peer_reset(peer);
        std::cout << "Connection failed\n";
        return EXIT_SUCCESS;
    }

    while(enet_host_service(client, &event, 1000) > 0)
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
        }
    }

    enet_peer_disconnect(peer, 0);
    while(enet_host_service(client, &event, 3000) > 0)
    {
        switch (event.type)
        {
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

    enet_host_destroy(client);
    return EXIT_SUCCESS;
}

int main(int argc, char **argv)
{
    if (enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);

    as_client();
}

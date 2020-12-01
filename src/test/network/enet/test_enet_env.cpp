#include <iostream>
#include <enet/enet.h>

int main(int argc, char **argv)
{
    if(enet_initialize() != 0)
    {
        std::cerr << "An error occurred while initializing ENet.\n";
        return EXIT_FAILURE;
    }
    atexit(enet_deinitialize);
}

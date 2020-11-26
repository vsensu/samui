#include <iostream>
#include <chrono>

#include <engine/network/net.h>

enum class CustomMsgTypes : uint32_t
{
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage,
};

class CustomClient : public samui::net::client_interface<CustomMsgTypes>
{
public:
    void ping()
    {
        samui::net::message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;

        std::chrono::system_clock::time_point time_now = std::chrono::system_clock::now();
        msg << time_now;
        this->send(msg);
    }
};

int main()
{
    CustomClient c;
    c.connect("127.0.0.1", 60000);
    c.ping();

    bool bQuit = false;
    while (!bQuit)
    {
        if(c.is_connected())
        {
            if(!c.incoming().empty())
            {
                auto msg = c.incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case CustomMsgTypes::ServerPing:
                {
                    auto time_now = std::chrono::system_clock::now();
                    decltype(time_now) time_then;
                    msg >> time_then;
                    std::cout << "Ping: " << std::chrono::duration<double>(time_now - time_then).count() << "\n";
                }
                    break;
                }
            }
        }
        else
        {
            std::cout << "Server Down\n";
            bQuit = true;
        }
        
    }
    

    return 0;
}

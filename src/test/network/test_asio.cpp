#include <iostream>
#include <vector>

#ifdef _WIN32
#define _WIN32_WINNT 0X0A00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

std::vector<char> vBuffer(20 * 1024);

void grab_some_data(asio::ip::tcp::socket &socket)
{
    socket.async_read_some(asio::buffer(vBuffer), [&](std::error_code ec, std::size_t length){
        if(!ec)
        {
            std::cout << "\n\nRead " << length << " bytes\n\n";
            for(int i = 0; i < length; ++i)
            {
                std::cout << vBuffer[i];
            }

            grab_some_data(socket);
        }
    });
}

int main()
{
    asio::error_code ec;

    asio::io_context context;

    asio::io_context::work idleWork(context);

    std::thread thrContext = std::thread([&](){ context.run(); });

    asio::ip::tcp::endpoint endpoint(asio::ip::make_address("93.184.216.34", ec), 80);

    asio::ip::tcp::socket socket(context);

    socket.connect(endpoint, ec);

    if(!ec)
    {
        std::cout << "Connected!\n";
    }
    else
    {
        std::cout << "Failed to connect to address:\n" << ec.message() << "\n";
    }

    if(socket.is_open())
    {
        grab_some_data(socket);

        std::string sRequest = 
        "GET /index.html HTTP/1.1\r\n"
        "Host: david-barr.co.uk\r\n"
        "Connection: close\r\n\r\n";
        socket.write_some(asio::buffer(sRequest), ec);

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);
    }

    return 0;
}
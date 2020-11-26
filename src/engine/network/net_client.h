#pragma once

#include <string>

#include "net_inc.h"
#include "net_message.h"
#include "net_tsqueue.h"
#include "net_connection.h"

namespace samui
{
    namespace net
    {
        template <typename T>
        class client_interface
        {
        public:
            client_interface() : m_socket(m_context)
            {

            }

            virtual ~client_interface()
            {
                disconnect();
            }

        public:
            bool connect(const std::string &host, const uint16_t port)
            {
                try
                {
                    asio::ip::tcp::resolver resolver(m_context);
                    asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));

                    m_connection = std::make_unique<connection<T>>(
                        connection<T>::owner::client,
                        m_context,
                        asio::ip::tcp::socket(m_context),
                        m_qMessagesIn
                    );
                    m_connection->connect_to_server(endpoints);
                    thrContext = std::thread([this](){m_context.run();});
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    return false;
                }
                
                return false;
            }

            void disconnect()
            {
                if(is_connected())
                {
                    m_connection->disconnect();
                }
                m_context.stop();
                if(thrContext.joinable())
                    thrContext.join();

                m_connection.release();
            }

            bool is_connected()
            {
                if(m_connection)
                    return m_connection->is_connected();

                return false;
            }

            tsqueue<owned_message<T>>& incoming()
            {
                return m_qMessagesIn;
            }

            void send(const message<T> &msg)
            {
                if(is_connected())
                {
                    m_connection->send(msg);
                }
            }

        protected:
            asio::io_context m_context;
            std::thread thrContext;
            asio::ip::tcp::socket m_socket;
            std::unique_ptr<connection<T>> m_connection;

        private:
            tsqueue<owned_message<T>> m_qMessagesIn;
        };
    }
}
#pragma once

#include "net_inc.h"
#include "net_tsqueue.h"
#include "net_message.h"
#include "net_connection.h"

namespace samui
{
    namespace net
    {
        template <typename T>
        class server_interface
        {
        public:
            server_interface(uint16_t port)
                : m_asioAcceptor(m_asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
            {

            }

            virtual ~server_interface()
            {
                stop();
            }

            bool start()
            {
                try
                {
                    wait_for_client_connection();
                    m_threadContext = std::thread([this](){m_asioContext.run();});
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    return false;
                }
                
                std::cout << "[SERVER] Started!\n";
                return true;
            }

            void stop()
            {
                m_asioContext.stop();

                if(m_threadContext.joinable())
                    m_threadContext.join();

                std::cout << "[SERVER] Stopped!\n";
            }

            void wait_for_client_connection()
            {
                m_asioAcceptor.async_accept(
                    [this](std::error_code ec, asio::ip::tcp::socket socket)
                    {
                        if(!ec)
                        {
                            std::cout << "[SERVER] New Connection: " << socket.remote_endpoint() << "\n";
                            std::shared_ptr<connection<T>> newconn = std::make_shared<connection<T>>(connection<T>::owner::server,
                                m_asioContext, std::move(socket), m_qMessagesIn);
                            if(on_client_connect(newconn))
                            {
                                m_deqConnections.push_back(std::move(newconn));
                                m_deqConnections.back()->connect_to_client(nIDCounter++);
                                std::cout << "[" << m_deqConnections.back()->GetID() << "] Connection Approved\n"; 
                            }
                            else
                            {
                                std::cout << "[-----] Connection Denied\n";
                            }
                        }
                        else
                        {
                            std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                        }

                        wait_for_client_connection();
                    }
                );
            }

            void message_client(std::shared_ptr<connection<T>> client, const message<T> &msg)
            {
                if(client && client->is_connected())
                {
                    client->send(msg);
                }
                else
                {
                    on_client_disconnect(client);
                    client.reset();
                    m_deqConnections.erase(
                        std::remove(m_deqConnections.begin(), m_deqConnections.end(), client), m_deqConnections.end());
                }
            }

            void message_all_clients(const message<T> &msg, std::shared_ptr<connection<T>> pIngoreClient = nullptr)
            {
                bool bInvalidExists = false;
                for(auto &client : m_deqConnections)
                {
                    if (client && client->is_connected())
                    {
                        if(client != pIngoreClient)
                            client->send(msg);
                    }
                    else
                    {
                        on_client_disconnect(client);
                        client.reset();
                        bInvalidExists = true;
                    }
                }

                if (bInvalidExists)
                {
                    m_deqConnections.erase(
                        std::remove(m_deqConnections.begin(), m_deqConnections.end(), nullptr), m_deqConnections.end());
                }
            }

            void update(size_t nMaxMessages = -1, bool bWait = false)
            {
                if(bWait) m_qMessagesIn.wait();

                size_t nMessageCount = 0;
                while(nMessageCount < nMaxMessages && !m_qMessagesIn.empty())
                {
                    auto msg = m_qMessagesIn.pop_front();
                    on_message(msg.remote, msg.msg);
                    ++nMessageCount;
                }
            }

        protected:
            virtual bool on_client_connect(std::shared_ptr<connection<T>> client)
            {
                return false;
            }

            virtual void on_client_disconnect(std::shared_ptr<connection<T>> client)
            {

            }

            virtual void on_message(std::shared_ptr<connection<T>> client, message<T> &msg)
            {

            }

        protected:
            tsqueue<owned_message<T>> m_qMessagesIn;
            std::deque<std::shared_ptr<connection<T>>> m_deqConnections;
            asio::io_context m_asioContext;
            std::thread m_threadContext;
            asio::ip::tcp::acceptor m_asioAcceptor;
            uint32_t nIDCounter = 10000;
        };
    }
}

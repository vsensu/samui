#pragma once

#include "net_inc.h"
#include "net_message.h"
#include "net_tsqueue.h"

namespace samui
{
    namespace net
    {
        template <typename T>
        class server_interface;

        template <typename T>
        class connection : public std::enable_shared_from_this<connection<T>>
        {
        public:
            enum class owner
            {
                server,
                client
            };

            connection(owner parent, asio::io_context &asioContext, asio::ip::tcp::socket socket, tsqueue<owned_message<T>> &qIn)
                : m_asioContext(asioContext), m_socket(std::move(socket)), m_qMessagesIn(qIn)
            {
                m_nOwnerType = parent;

                if(m_nOwnerType == owner::server)
                {
                    m_nHandshakeOut = uint64_t(std::chrono::system_clock::now().time_since_epoch().count());

                    m_nHandshakeCheck = scramble(m_nHandshakeOut);
                }
                else
                {
                    m_nHandshakeIn = 0;
                    m_nHandshakeOut = 0;
                }
            }

            virtual ~connection() {}

            uint32_t GetID() const { return id; }

        public:
            void connect_to_client(samui::net::server_interface<T> *server, uint32_t uid = 0)
            {
                if (m_nOwnerType == owner::server)
                {
                    if (m_socket.is_open())
                    {
                        id = uid;
                        write_validation();
                        read_validation(server);
                    }
                }
            }

            void connect_to_server(asio::ip::tcp::resolver::results_type &endpoints)
            {
                if(m_nOwnerType == owner::client)
                {
                    asio::async_connect(m_socket, endpoints,
                    [this](std::error_code ec, asio::ip::tcp::endpoint endpoint)
                    {
                        if(!ec)
                        {
                            read_validation();
                        }
                    }
                    );
                }
            }

            void disconnect()
            {
                if(is_connected())
                {
                    asio::post(m_asioContext, [this](){m_socket.close();});
                }
            }

            bool is_connected() const
            {
                return m_socket.is_open();
            }

        public:
            void send(const message<T> &msg)
            {
                asio::post(m_asioContext, 
                [this, msg](){
                    bool bWritingMessage = !m_qMessagesOut.empty();
                    m_qMessagesOut.push_back(msg);
                    if(!bWritingMessage)
                    {
                        write_header();
                    }
                });
            }

        private:
            void read_header()
            {
                asio::async_read(m_socket, asio::buffer(&m_msgTempIn.header, sizeof(decltype(m_msgTempIn)::header)),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec)
                                     {
                                         if (m_msgTempIn.header.size > 0)
                                         {
                                             m_msgTempIn.body.resize(m_msgTempIn.header.size);
                                             read_body();
                                         }
                                         else
                                         {
                                             add_to_incoming_message_queue();
                                         }
                                     }
                                     else
                                     {
                                         std::cout << "[" << id << "] Read Header Fail.\n";
                                         m_socket.close();
                                     }
                                 });
            }

            void read_body()
            {
                asio::async_read(m_socket, asio::buffer(m_msgTempIn.body),
                                 [this](std::error_code ec, std::size_t length) {
                                     if (!ec)
                                     {
                                         add_to_incoming_message_queue();
                                     }
                                     else
                                     {
                                         std::cout << "[" << id << "] Read Body Fail.\n";
                                         m_socket.close();
                                     }
                                 });
            }

            void write_header()
            {
                asio::async_write(m_socket, asio::buffer(&m_qMessagesOut.front().header, sizeof(message_header<T>)),
                                  [this](std::error_code ec, std::size_t length) {
                                      if (!ec)
                                      {
                                          if (m_qMessagesOut.front().body.size() > 0)
                                          {
                                              write_body();
                                          }
                                          else
                                          {
                                              m_qMessagesOut.pop_front();
                                              if(!m_qMessagesOut.empty())
                                              {
                                                  write_header();
                                              }
                                          }
                                      }
                                      else
                                      {
                                          std::cout << "[" << id << "] Write Header Fail.\n";
                                          m_socket.close();
                                      }
                                  });
            }

            void write_body()
            {
                asio::async_write(m_socket, asio::buffer(m_qMessagesOut.front().body),
                                  [this](std::error_code ec, std::size_t length) {
                                      if (!ec)
                                      {
                                          if (m_qMessagesOut.front().body.size() > 0)
                                          {
                                              write_body();
                                          }
                                          else
                                          {
                                              m_qMessagesOut.pop_front();
                                              if(!m_qMessagesOut.empty())
                                              {
                                                  write_header();
                                              }
                                          }
                                      }
                                      else
                                      {
                                          std::cout << "[" << id << "] Write Body Fail.\n";
                                          m_socket.close();
                                      }
                                  });
            }

            void add_to_incoming_message_queue()
            {
                if(m_nOwnerType == owner::server)
                {
                    m_qMessagesIn.push_back({this->shared_from_this(), m_msgTempIn});
                }
                else
                {
                    m_qMessagesIn.push_back({nullptr, m_msgTempIn});
                }

                read_header();
            }

            uint64_t scramble(uint64_t nInput)
            {
                uint64_t out = nInput ^ 0xDEADBEEFC0DECAFE;
                out = (out & 0xF0F0F0F0F0F0F0) >> 4 | (out & 0x0F0F0F0F0F0F0F) << 4;
                return out ^ 0xC0DEFACE12345678;
            }

            void write_validation()
            {
                asio::async_write(m_socket, asio::buffer(&m_nHandshakeOut, sizeof(uint64_t)),
                    [this](std::error_code ec, std::size_t length) {
                        if(!ec)
                        {
                            if(m_nOwnerType == owner::client)
                            {
                                read_header();
                            }
                        }
                        else
                        {
                            m_socket.close();
                        }
                    }
                );
            }

            void read_validation(samui::net::server_interface<T> *server = nullptr)
            {
                asio::async_read(m_socket, asio::buffer(&m_nHandshakeIn, sizeof(uint64_t)), 
                    [this, server](std::error_code ec, std::size_t length)
                    {
                        if(!ec)
                        {
                            if(m_nOwnerType == owner::server)
                            {
                                if(m_nHandshakeIn == m_nHandshakeCheck)
                                {
                                    std::cout << "Client Validated\n";
                                    server->on_client_validated(this->shared_from_this());
                                    read_header();
                                }
                                else
                                {
                                    std::cout << "Client Disconnected (Fail Validation)\n";
                                    m_socket.close();
                                }
                            }
                            else
                            {
                                m_nHandshakeOut = scramble(m_nHandshakeIn);

                                write_validation();
                            }
                            
                        }
                        else
                        {
                            std::cout << "Client Disconnected (ReadValidation)\n";
                            m_socket.close();
                        }
                        
                    }
                );
            }

        protected:
            asio::ip::tcp::socket m_socket;
            asio::io_context &m_asioContext;
            tsqueue<message<T>> m_qMessagesOut;
            tsqueue<owned_message<T>> &m_qMessagesIn;
            message<T> m_msgTempIn;
            owner m_nOwnerType = owner::server;
            uint32_t id = 0;

            uint64_t m_nHandshakeIn, m_nHandshakeOut, m_nHandshakeCheck;
        };
    } // namespace net
} // namespace samui

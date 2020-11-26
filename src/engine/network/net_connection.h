#pragma once

#include "net_inc.h"
#include "net_message.h"
#include "net_tsqueue.h"

namespace samui
{
    namespace net
    {
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
            }

            virtual ~connection() {}

            uint32_t GetID() const { return id; }

        public:
            void connect_to_client(uint32_t uid = 0)
            {
                if (m_nOwnerType == owner::server)
                {
                    if (m_socket.is_open())
                    {
                        id = uid;
                        read_header();
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
                            read_header();
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

        protected:
            asio::ip::tcp::socket m_socket;
            asio::io_context &m_asioContext;
            tsqueue<message<T>> m_qMessagesOut;
            tsqueue<owned_message<T>> &m_qMessagesIn;
            message<T> m_msgTempIn;
            owner m_nOwnerType = owner::server;
            uint32_t id = 0;
        };
    } // namespace net
} // namespace samui

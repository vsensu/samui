#pragma once

#include <vector>
#include <ostream>

namespace samui
{
    namespace net
    {
        template <typename T>
        struct message_header
        {
            T id{};
            uint32_t size = 0;
        };

        template <typename T>
        struct message
        {
            message_header<T> header{};
            std::vector<uint8_t> body;

            size_t size() const 
            {
                return sizeof(message_header<T>) + body.size();
            }

            friend std::ostream& operator << (std::ostream &os, const message<T> &msg)
            {
                os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
                return os;
            }

            template <typename PODDataType>
            friend message<T>& operator << (message<T> &msg, const PODDataType &data)
            {
                static_assert(std::is_standard_layout<PODDataType>::value, "Data is too complex");
                size_t i = msg.body.size();
                msg.body.resize(msg.body.size() + sizeof(PODDataType));
                std::memcpy(msg.body.data() + i, &data, sizeof(PODDataType));
                msg.header.size = msg.size();
                return msg;
            }

            template <typename PODDataType>
            friend message<T>& operator >> (message<T> &msg, PODDataType &data)
            {
                static_assert(std::is_standard_layout<PODDataType>::value, "Data is too complex");
                size_t i = msg.body.size() - sizeof(PODDataType);
                std::memcpy(&data, msg.body.data() + i, sizeof(PODDataType));
                msg.body.resize(i);
                msg.header.size = msg.size();
                return msg;
            }
        };
    }
}
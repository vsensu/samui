#pragma once

// clang-format off
#include <sstream>
#include <string>

#include <yaml-cpp/yaml.h>

#include "serialization_module.h"
// clang-format on

namespace samui
{

namespace serialization
{

template <typename T>
YAML::Emitter& operator<<(YAML::Emitter& out, const T& v)
{
    out << YAML::convert<T>::encode(v);
    return out;
}

template <typename T>
YAML::Emitter& serialize(YAML::Emitter& out, const T& value)
{
    out << value;
    return out;
}

template <typename T>
std::string serialize(const T& value)
{
    YAML::Emitter out;
    serialize(out, value);
    return out.c_str();
}

struct SAMUI_SERIALIZATION_API Header
{
    int version;
};

SAMUI_SERIALIZATION_API YAML::Emitter& write_header(YAML::Emitter& out, const Header &value);

template <typename T>
void serialize_to_file(const T& value, const std::string& filepath)
{
    YAML::Emitter out;
    Header        header;
    header.version = 1;
    write_header(out, value);
    serialize(out, value);
    std::ofstream fout(filepath);
    fout << out.c_str();
}

template <typename T>
using convert = YAML::convert<T>;

}  // namespace serialization

}  // namespace samui

namespace YAML
{

template <>
struct convert<::samui::serialization::Header>
{
    static Node encode(const ::samui::serialization::Header& rhs)
    {
        Node node;
        node.push_back(rhs.version);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, ::samui::serialization::Header& rhs)
    {
        if (!node.IsSequence() || node.size() != 1) return false;

        rhs.version = node[0].as<int>();
        return true;
    }
};

}  // namespace YAML

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

/**
 * @brief specialization for custom types 
 * 
 * @tparam T 
 * @param value 
 * @return std::string 
 */
template <typename T>
std::string to_serialize(const T &value)
{
    return "";
}

std::string SAMUI_SERIALIZATION_API write_header(YAML::Emitter& out);

template <typename T>
std::string serialize(const T &value)
{
    YAML::Emitter out;
    std::stringstream ss;

    // write header
    ss << write_header(out);

    // write content
    ss << to_serialize(value);

    return ss.str();
}

}  // namespace serialization

}  // namespace samui

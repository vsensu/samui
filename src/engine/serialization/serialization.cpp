#include "serialization.h"

namespace samui::serialization
{

std::string write_header(YAML::Emitter& out)
{
    // write version
    out << YAML::Key << "version" << YAML::Value << "1.0";
    return out.c_str();
}

}
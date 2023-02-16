#include "serialization.h"

namespace samui::serialization
{

YAML::Emitter& write_header(YAML::Emitter& out, const Header &value)
{
    return out << value;
}

}
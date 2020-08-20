#pragma once

#include "value_source.hpp"

#include <string>

namespace confy
{

struct generic_value
{
    std::string value;

    // true if the value here is the default value of an option
    // note that the source might be different than default_val
    // a explicit config might push for default
    bool is_default = false;

    value_source source = value_source::none;
};

}

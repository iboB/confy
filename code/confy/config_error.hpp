#pragma once

#include "value_source.hpp"
#include <string>

namespace confy
{

class config_error
{
public:
    value_source source;
    std::string source_name;

    enum type_
    {
        bad_source, // the source of the value was malformed
        no_such_section, // configuration referred to a section which was not defined in schema
        no_such_option, // configuration tried to set value to an option that was not defined in schema
        bad_default_value, // default value of option was missing or incompatible
        bad_value, // provided value of option was incompatible
        multiple_value, // value was provided multiple times from the same source
    };
    type_ type;

    std::string section_name; // contains the referred section name
    std::string option_name; // contains the referred option name
    std::string provided_value; // what value was provided from the config
    const option* opt = nullptr; // if not null, will contain the option which was referred

    // free-form text of the error
    std::string error_text;
};

}

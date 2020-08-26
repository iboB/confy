// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "value_source.hpp"
#include <string>

namespace confy
{

// config events are not exceptions
// all errors will be collected into a vector and can be queried
// optionally if verbose is on all events are listed here
// potentially an app might want to continue working with error
class config_event
{
public:
    value_source source;
    std::string source_name;

    enum type_
    {
        bad_source, // the source of the value was malformed
        no_such_section, // configuration referred to a section which was not defined in schema
        no_such_option, // configuration tried to set value to an option that was not defined in schema
        same_source_value, // value was provided multiple times from the same source
        bad_value, // provided value of option was incompatible
        bad_default, // default value of option was missing or incompatible
    };
    type_ type;

    std::string section_name; // contains the referred section name
    std::string option_name; // contains the referred option name
    std::string provided_value; // what value was provided from the config
    const option* opt = nullptr; // if not null, will contain the option which was referred

    // optional free-form text of the event
    std::string text;
};

}

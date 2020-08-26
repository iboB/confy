// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "value_source.hpp"
#include "option_set_value_result.hpp"

#include <string>
#include <optional>

namespace confy
{

// config events are not exceptions
// all errors will be collected into a vector and can be queried
// optionally if verbose is on all events are listed here
// potentially an app might want to continue working with error
class config_event
{
public:
    enum event_type
    {
        verbose, // not an error. Only created if verbose is on
        source_error, // the source was malformed
        missing_error, // configuration referred to a section or option which was not defined in the schema
        set_error, // the setting of an option failed
    };
    event_type type = verbose;

    bool is_error() const { return type > verbose; }

    value_source source; // data source of error
    std::string source_name; // name of source

    bool is_abbr = false; // are the section and option names abbreviations
    std::optional<std::string> section_name; // contains the referred section name if any
    std::optional<std::string> option_name; // contains the referred option name if any

    std::optional<std::string> provided_value; // what value was provided from the config if any

    const option* opt = nullptr; // if not null, will contain the option which was referred
    std::optional<option_set_value_result> set_value_result; // the result of the set value op if any

    // optional free-form text of the event
    // confy only writes here when parsing ini files
    // otherwise it can be filled by a user-provided extension
    std::optional<std::string> text;
};

}

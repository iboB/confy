// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

namespace confy
{

enum class option_set_value_result
{
    success, // value was set
    skipped, // setting a value from a smaller source (lower prio). soft success
    same_source_value, // a value was provided from the same source
    bad_value, // the provided value was not compatible with the type
    bad_default, // default was requested but the option has a missing or incompatible default

    // not returned by default try_set_value implementation
    bad_source, // the provided source is not allowed for this value
    abort_source, // stop parsing the current source
    command_error, // error executing a command
};

}

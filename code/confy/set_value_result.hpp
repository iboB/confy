#pragma once

namespace confy
{

enum class set_value_result
{
    success, // value was set
    skipped, // setting a value from a smaller source (lower prio). soft success
    bad_source, // the provided source is not allowed for this value
    same_source_value, // a value was provided from the same source
    bad_value, // the provided value was not compatible with the type
    bad_default, // default was requested but the option has a missing or incompatible default
};

}

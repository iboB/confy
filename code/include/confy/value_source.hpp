#pragma once

namespace confy
{

// sorted by priority lower to higher
enum class value_source
{
    none, // value has no source (usually this means there is no value)
    default_val, // manually entered default value from the config dsl
    env_var, // was read from an environment variable
    config_file, // was read from a configuration file which was loaded at some point
    cmd_line, // explicitly set via a command line arg
    manual_override, // manually overrided by the user
};

}

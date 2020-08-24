#pragma once

#include "impl/api.h"

#include "impl/config_item.hpp"

#include "value_source.hpp"

#include <string>
#include <iosfwd>

namespace confy
{

class config;
class section;

class CONFY_API option : public impl::config_item
{
public:
    virtual ~option();

    const std::string& env_var() const { return m_env_var; }

    bool no_env() const { return m_no_env; }

    value_source source() const { return m_source; }

    section* sec() const { return m_section; }

    enum class set_value_result
    {
        success,
        skipped, // setting a value from a smaller source (lower prio). soft success
        same_source_value, // setting a value from the same source
        bad_value, // value was incompatible
        bad_default, // default was missing or incompatible
    };

    // the default implementation will try to set the value from the given source
    // if the value is "default", it will try to set the default value
    // if the value set is a success, it will set m_source to the source
    virtual set_value_result try_set_value(std::string_view val, value_source source);

    // command line and printing utils

    // you can override this with `return true` on boolean option with false defaults
    // this will provide a prettier schema output
    virtual bool true_only() const { return false; }

    // print the value description
    virtual void write_value_desc(std::ostream& out) const = 0;

    // you can override this for options with no default value
    virtual bool has_default_val() const { return true; }

    // write the default value
    virtual void write_default_val(std::ostream& out) const = 0;

protected:
    friend class config;
    friend class section;

    virtual bool set_from_default() = 0;
    virtual bool set_from_string(std::string_view str) = 0;

    std::string m_description; // free form description
    std::string m_env_var; // associated environment variable name
    bool m_no_env = false; // don't use env var for this option only

    value_source m_source = value_source::none; // source from which the value was set

    section* m_section = nullptr;
};

}

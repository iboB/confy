// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once

#include "impl/api.h"

#include "impl/config_item.hpp"

#include "value_source.hpp"
#include "schema_dsl.hpp"
#include "option_set_value_result.hpp"

#include <string>
#include <iosfwd>

namespace confy
{

class config;
class section;

class CONFY_API option : public impl::config_item
{
public:
    using config_item::config_item;
    virtual ~option();

    const std::string& env_var() const { return m_env_var; }
    void set_env_var(std::string_view v) { m_env_var = v; }

    bool no_env() const { return m_no_env; }
    void set_no_env(bool s = true) { m_no_env = s; }

    value_source source() const { return m_source; }

    section* sec() const { return m_section; }

    using set_value_result = option_set_value_result;

    // the default implementation will try to set the value from the given source
    // if the value is "default", it will try to set the default value
    // if the value set is a success, it will set m_source to the source
    // you can override this function and guard for specifc sources and execute code
    // this is useful for command-like options
    virtual set_value_result try_set_value(std::string_view val, value_source source);

    // dsl to define a schema
    // it's instantiated by the schema_dsl opt/cmd functions
    template <typename Option>
    struct dsl_t
    {
    public:
        dsl_t(Option& o, schema_dsl& dsl) : m_option(o), m_schema_dsl(dsl) {}

        using dsl = typename Option::dsl;

        dsl& name(std::string_view n) { m_option.set_name(n); return self(); }
        dsl& abbr(std::string_view n) { m_option.set_abbr(n); return self(); }
        dsl& desc(std::string_view n) { m_option.set_description(n); return self(); }
        dsl& env(std::string_view n)
        {
            if (n.empty()) m_option.set_no_env();
            else m_option.set_env_var(n);
            return self();
        }

        // following are redirecting functions which just call the schema_dls's ones
        // so one can write .opt(...).opt(...).sec(...).opt(...) to define multiple options and sections
        // withing the same schema

        schema_dsl& sec(std::string_view name, std::string_view abbr = {}, std::string_view desc = {})
        {
            return m_schema_dsl.sec(name, abbr, desc);
        }
        template <typename Other>
        auto cmd(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
        {
            return m_schema_dsl.cmd<Other>(name, abbr, desc);
        }
        template <typename Other, typename Value>
        auto opt(Value& val, std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
        {
            return m_schema_dsl.opt<Other, Value>(val, name, abbr, desc);
        }
        template <typename Value>
        auto opt(Value& val, std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
        {
            return m_schema_dsl.opt(val, name, abbr, desc);
        }

    protected:
        dsl& self()
        {
            return static_cast<dsl&>(*this);
        }
        Option& m_option;
    private:
        schema_dsl& m_schema_dsl;
    };

    using dsl = dsl_t<option>;

    // command line and printing utils

    // you can override this with `return true` on boolean option with false defaults
    // this will provide a prettier schema output
    virtual bool is_default_false() const { return false; }

    // print the value type
    virtual void write_value_type(std::ostream& out) const = 0;

    // print the value type description
    virtual void write_value_type_desc(std::ostream& out) const = 0;

    // you can override this for options with no default value
    virtual bool has_default_value() const = 0;

    // write the default value
    virtual void write_default_value(std::ostream& out) const = 0;

protected:
    friend class config;
    friend class section;

    // return true on success, false otherwise
    virtual bool set_from_default() = 0;
    virtual bool set_from_string(std::string_view str) = 0;

    std::string m_env_var; // associated environment variable name
    bool m_no_env = false; // don't use env var for this option only

    value_source m_source = value_source::none; // source from which the value was set

    section* m_section = nullptr;
};

}

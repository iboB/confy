// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "dict_fwd.hpp"
#include "value_source.hpp"
#include "env.hpp"
#include <string>
#include <string_view>

namespace confy {

class section;

class CONFY_API basic_value {
public:
    virtual ~basic_value() noexcept;

    const section* owner() const noexcept { return m_section; }
    const std::string& name() const noexcept { return m_name; }
    const std::string& desc() const noexcept { return m_desc; }
    const std::string& abbr() const noexcept { return m_abbr; }

    virtual std::string to_string() const noexcept = 0;
    virtual dict to_dict() const noexcept = 0;

    virtual void set_from_string(std::string_view str) = 0;

    // the default implementation simply returns false (no default value)
    virtual bool try_set_from_default() noexcept;

    // the default implementation will try to set from string if applicable
    // and throw an exception if it fails
    virtual void set_from_dict(const dict& d);

    // throw an exception if the value is not valid
    // the default implementation does nothing
    virtual void validate() const;

    // return a human readable string description of the value validation
    // the default implementation returns an empty string
    virtual std::string get_validation_desc() const noexcept;

protected:
    friend class configurator;
    friend class section;
    template <typename Value, typename Crtp>
    friend class basic_value_dsl;

    const section* m_section = nullptr;

    std::string m_name;
    std::string m_desc;
    std::string m_abbr;
    env::var m_env_var;
    bool required = false; // whether this value is required or optional

    value_source m_source = value_source::none; // source from which the value was set
};

} // namespace confy
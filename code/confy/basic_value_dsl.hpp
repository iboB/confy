// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string_view>

namespace confy {

template <typename Value, typename Crtp> // C++23 replace crtp with deducing this
class basic_value_dsl {
public:
    Value& value;

    basic_value_dsl(Value& value) : value(value) {}

    Crtp& name(std::string_view n) {
        value.m_name = std::string(n);
        return self();
    }

    Crtp& desc(std::string_view d) {
        value.m_desc = std::string(d);
        return self();
    }

    Crtp& abbr(std::string_view a) {
        value.m_abbr = std::string(a);
        return self();
    }

    Crtp& env_var(std::string_view e) {
        value.m_env_var = std::string(e);
        value.m_env_var_type = Value::env_var_type::manual;
        return self();
    }

    Crtp& env_var_global(std::string_view e) {
        value.m_env_var = std::string(e);
        value.m_env_var_type = Value::env_var_type::manual_global;
        return self();
    }

    Crtp& no_env_var() {
        value.m_env_var_type = Value::env_var_type::none;
        return self();
    }

    Crtp& required() {
        value.required = true;
        return self();
    }

protected:
    Crtp& self() { return static_cast<Crtp&>(*this); }
};

} // namespace confy

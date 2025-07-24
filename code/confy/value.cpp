// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "value.hpp"
#include "bits/throw_ex.hpp"
#include <cstdlib>

namespace confy {

void value::set_from_string(std::string_view str, value_source src) {
    if (src < m_source) return;
    try {
        set_value_from_string(str);
    }
    catch (confy::exception& ex) {
        rethrow(ex);
    }
    m_source = src;
}

void value::set_from_dict(const dict& d, value_source src) {
    if (src < m_source) return;
    try {
        set_value_from_dict(d);
    }
    catch (confy::exception& ex) {
        rethrow(ex);
    }
    m_source = src;
}

void value::validate() const {
    if (m_source == value_source::none) {
        if (m_required) {
            throw_ex{} << get_path() << ": value is required but not set";
        }
        // nothing to validate
        return;
    }

    try {
        validate_value();
    }
    catch (confy::exception& ex) {
        rethrow(ex);
    }
}

void value::try_set_from_env_var() {
    if (m_source >= value_source::env_var) return;
    auto env_var_name = get_env_var_name();

    auto env_value = std::getenv(env_var_name.c_str());
    if (env_value) {
        try {
            set_value_from_string(env_value);
            m_source = value_source::env_var;
        }
        catch (confy::exception& e) {
            throw_ex{} << get_path() << ": " << e.what() << " from env_var "
                << env_var_name + " = " << env_value;
        }
    }
}

void value::rethrow(const confy::exception& ex) const {
    throw_ex{} << get_path() << ": " << ex.what();
}

} // namespace confy

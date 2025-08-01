// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "value.hpp"
#include "node_visitor.hpp"
#include "bits/throw_ex.hpp"
#include <cstdlib>
#include <cstring>

namespace confy {

void value::set_from_string(std::string_view str, value_source src) {
    if (src < m_source) return;
    try {
        set_value_from_string(str);
    }
    catch (std::exception& ex) {
        rethrow(ex.what());
    }
    m_source = src;
}

void value::set_from_dict(const dict& d, value_source src) {
    if (src < m_source) return;
    try {
        set_value_from_dict(d);
    }
    catch (std::exception& ex) {
        rethrow(ex.what());
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
    catch (std::exception& ex) {
        std::ostringstream ss;
        ss << "value '" << to_string() << "' failed validation";
        auto desc = ex.what();
        if (std::strlen(desc) > 0) {
            ss << ": " << desc;
        }
        rethrow(ss.str().c_str());
    }
}

void value::visit(node_visitor& v) const {
    v.on(*this);
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
            throw_ex{} << get_path() << ": " << e.what() << " from env var "
                << env_var_name + "=" << env_value;
        }
    }
}

void value::rethrow(const char* ex) const {
    throw_ex{} << get_path() << ": " << ex;
}

} // namespace confy

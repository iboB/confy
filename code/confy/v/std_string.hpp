// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"

namespace confy {

template <>
struct common_value_type_traits<std::string> {
    using ref_type = std::string&;
    using const_ref_type = const std::string&;
    using validate_type = std::string;
    static inline constexpr bool deref = false;

    static const std::string& to_string(const std::string& val) noexcept {
        return val;
    }

    static dict to_dict(const std::string& val) noexcept {
        return val;
    }

    static void set_value_from_string(std::string& val, std::string_view str) {
        val = std::string(str);
    }

    static void set_value_from_dict(std::string& val, const dict& d) {
        val = d.get<std::string>();
    }
};

using std_string = common_value<std::string>;

}

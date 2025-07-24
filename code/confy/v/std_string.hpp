// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../ref_value_for.hpp"

namespace confy {

template <>
struct common_value_type_traits<std::string> {
    using value_type = std::string;
    using ref_type = std::string&;
    using const_ref_type = const std::string&;

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

template <>
struct ref_value_for<std::string> {
    using type = common_value<std::string&>;
};

}

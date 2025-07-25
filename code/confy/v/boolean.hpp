// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../bits/throw_ex.hpp"

namespace confy {

template <>
struct common_value_type_traits<bool> {
    using ref_type = bool&;
    using const_ref_type = const bool&;
    using validate_type = bool;
    static inline constexpr bool deref = false;

    static std::string to_string(bool val) noexcept {
        return val ? "true" : "false";
    }

    static dict to_dict(bool val) noexcept {
        return val;
    }

    static void set_value_from_string(bool& val, std::string_view str) {
        if (str == "true" || str == "1") {
            val = true;
        }
        else if (str == "false" || str == "0") {
            val = false;
        }
        else {
            throw_ex{} << "failed to parse boolean from '" << str << "'";
        }
    }

    static void set_value_from_dict(bool& val, const dict& d) {
        val = d.get<bool>();
    }
};

using boolean = common_value<bool>;

}

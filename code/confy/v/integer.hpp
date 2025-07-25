// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../bits/throw_ex.hpp"
#include <concepts>
#include <charconv>

namespace confy {

template <std::integral Int>
struct common_value_type_traits<Int> {
    using ref_type = Int&;
    using const_ref_type = const Int&;
    using validate_type = Int;
    static inline constexpr bool deref = false;

    static std::string to_string(const Int& val) noexcept {
        return std::to_string(val);
    }

    static dict to_dict(const Int& val) noexcept {
        dict ret;
        ret = dict::number_integer_t(val);
        return ret;
    }

    static void set_value_from_string(Int& val, std::string_view str) {
        auto end = str.data() + str.size();
        auto res = std::from_chars(str.data(), end, val);
        if (res.ec != std::errc() || res.ptr != end) {
            throw_ex{} << "failed to parse " << sizeof(Int) * 8 << "-bit integer from '" << str << "'";
        }
    }

    static void set_value_from_dict(Int& val, const dict& d) {
        val = d.get<Int>();
    }
};

template <std::integral Int>
using integer = common_value<Int>;

} // namespace confy

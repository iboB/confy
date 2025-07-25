// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../bits/throw_ex.hpp"
#include <concepts>

#include "../bits/magic_enum.hpp"

#if !MAGIC_ENUM_SUPPORTED
#   error This compiler doesn't support magic_enum, so it can't support confy::auto_enum
#endif

namespace confy {

template <typename T>
concept enumeration = std::is_enum_v<T>;

template <enumeration E>
struct common_value_type_traits<E> {
    using ref_type = E&;
    using const_ref_type = const E&;
    using validate_type = E;
    static inline constexpr bool deref = false;

    static std::string to_string(const E& val) noexcept {
        return std::string(magic_enum::enum_name(val));
    }

    static dict to_dict(const E& val) noexcept {
        return to_string(val);
    }

    static void set_value_from_string(E& val, std::string_view str) {
        auto oval = magic_enum::enum_cast<E>(str);
        if (!oval.has_value()) {
            throw_ex{} << "failed to parse enum from '" << str << "'";
        }
        val = *oval;
    }

    static void set_value_from_dict(E& val, const dict& d) {
        auto sv = d.get<std::string_view>();
        set_value_from_string(val, sv);
    }
};

template <enumeration E>
using auto_enum = common_value<E>;

template <enumeration E>
struct ref_value_for<E> {
    using type = common_value<E&>;
};

}

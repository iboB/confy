// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../bits/throw_ex.hpp"
#include <concepts>
#include <charconv>

namespace confy {

template <typename T>
struct common_value_type_traits<std::optional<T>> {
    using ref_type = std::optional<T>&;
    using const_ref_type = const std::optional<T>&;
    using validate_type = T;
    static inline constexpr bool deref = true;

    using value_traits = common_value_type_traits<T>;

    static std::string to_string(const std::optional<T>& val) noexcept {
        if (!val) {
            return "null";
        }
        return value_traits::to_string(*val);
    }

    static dict to_dict(const std::optional<T>& val) noexcept {
        if (!val) {
            return {};
        }
        return value_traits::to_dict(*val);
    }

    static void set_value_from_string(std::optional<T>& val, std::string_view str) {
        if (str == "null") {
            val.reset();
            return;
        }
        if (!val) {
            val.emplace();
        }
        return value_traits::set_value_from_string(*val, str);
    }

    static void set_value_from_dict(std::optional<T>& val, const dict& d) {
        if (d.is_null()) {
            val.reset();
            return;
        }
        if (!val) {
            val.emplace();
        }
        return value_traits::set_value_from_dict(*val, d);
    }
};

template <typename Opt>
class std_optional_value : public common_value<Opt> {
public:
    using super = common_value<Opt>;
    using super::common_value;

    void validate_value() const override {
        if (!this->m_val) return; // nothing to validate if not set
        for (auto& v : this->m_validators) {
            v->validate(*this->m_val);
        }
    }
};

template <typename T>
using std_optional = std_optional_value<std::optional<T>>;

template <typename T>
struct ref_value_for<std::optional<T>> {
    using type = std_optional_value<std::optional<T>&>;
};

} // namespace confy

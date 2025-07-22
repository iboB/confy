// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_value.hpp"
#include "../dict.hpp"
#include "../bits/value_ex.hpp"
#include <concepts>
#include <charconv>

namespace confy {

template <std::integral Int>
class integer : public common_value<Int> {
public:
    using common_value<Int>::common_value;

    std::string to_string() const noexcept {
        return std::to_string(this->m_val);
    }

    virtual void set_from_string(std::string_view str) override {
        auto end = str.data() + str.size();
        auto res = std::from_chars(str.data(), end, this->m_val);
        if (res.ec != std::errc() || res.ptr != end) {
            value_ex{this} << "failed to parse " << sizeof(Int) * 8 << "-bit integer from '" << str << "'";
        }
    }

    virtual void set_from_dict(const dict& d) override {
        this->m_val = d.get<Int>();
    }
};

template <std::integral Int>
std::unique_ptr<integer<Int>> make_value(Int& val) {
    return std::make_unique<integer<Int>>(val);
}

} // namespace confy

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_ref_value.hpp"
#include "simple_value.hpp"
#include "../dict.hpp"
#include "../bits/throw_ex.hpp"
#include <concepts>
#include <charconv>

namespace confy {

template <std::integral Int>
class integer_ref final : public common_ref_value<Int> {
public:
    using common_ref_value<Int>::common_ref_value;

    std::string to_string() const noexcept override {
        return std::to_string(this->m_ref);
    }

    dict to_dict() const noexcept override {
        dict ret;
        ret = dict::number_integer_t(this->m_ref);
        return ret;
    }

    virtual void set_value_from_string(std::string_view str) override {
        auto end = str.data() + str.size();
        auto res = std::from_chars(str.data(), end, this->m_ref);
        if (res.ec != std::errc() || res.ptr != end) {
            throw_ex{} << this->get_path() << ": failed to parse " << sizeof(Int) * 8 << "-bit integer from '" << str << "'";
        }
    }

    virtual void set_value_from_dict(const dict& d) override {
        this->m_ref = d.get<Int>();
    }
};

template <std::integral Int>
struct ref_value_for<Int> {
    using type = integer_ref<Int>;
};

template <std::integral Int>
using integer = simple_value<Int>;

} // namespace confy

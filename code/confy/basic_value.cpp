// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "basic_value.hpp"
#include "dict.hpp"
#include "bits/value_ex.hpp"

namespace confy {

basic_value::~basic_value() noexcept = default;

bool basic_value::try_set_from_default() noexcept { return false; }

void basic_value::set_from_dict(const dict& d) {
    // try to set if the dict is a string
    if (d.type() != dict::value_t::string) {
        value_ex{this} << "can't set value as string from dict of type " << d.type_name();
    }

    set_from_string(d.get<std::string_view>());
}

void basic_value::validate() const {}

std::string basic_value::get_validation_desc() const noexcept { return {}; }

} // namespace confy

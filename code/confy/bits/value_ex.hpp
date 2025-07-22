// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "throw_ex.hpp"
#include "../section.hpp"
#include "../basic_value.hpp"

namespace confy {
struct value_ex : public throw_ex {
    value_ex(const basic_value& v) {
        if (auto sec = v.sec()) {
            *this << sec->desc().name << ".";
        }
        *this << v.name() << ": ";
    }
    value_ex(const basic_value* v) : value_ex(*v) {}
};
}

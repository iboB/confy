// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../ref_value_for.hpp"

namespace confy {

template <typename T>
class simple_value : public ref_value_for_t<T> {
    T m_val;
public:
    using super = ref_value_for_t<T>;

    simple_value(node_desc desc, node* owner)
        : super(m_val, std::move(desc), owner)
        , m_val()
    {}

   // this constructor is used for testing
    explicit simple_value(T& ref)
        : simple_value({}, nullptr)
    {}
};

} // namespace confy


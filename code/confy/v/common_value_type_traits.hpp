// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace confy {

template <typename T>
struct common_value_type_traits;

template <typename T>
struct common_value_type_traits<T&> : common_value_type_traits<T> {};

} // namespace confy
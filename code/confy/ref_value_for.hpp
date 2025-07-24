// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once

namespace confy {

template <typename T>
struct ref_value_for;

template <typename T>
using ref_value_for_t = typename ref_value_for<T>::type;

} // namespace confy

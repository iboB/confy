// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../exception.hpp"
#include <itlib/throw_ex.hpp>

namespace confy {
using throw_ex = itlib::throw_ex<exception>;
}

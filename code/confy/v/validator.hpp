// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string>

namespace confy {

template <typename T>
struct validator {
    virtual ~validator() noexcept = default;
    virtual void validate(const T& value) const = 0;
    virtual std::string get_desc() const noexcept = 0;
};

} // namespace confy

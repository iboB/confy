// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"

namespace confy {

class CONFY_API value : public node {
public:
    void validate() const final override;

protected:
    virtual void validate_value() const = 0;

    bool required = false; // whether this value is required or optional
    value_source m_source = value_source::none; // source from which the value was set
};

} // namespace confy

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"

namespace confy {

class section;
class value;

class CONFY_API node_visitor {
public:
    virtual ~node_visitor() noexcept;

    virtual void on(const section& sec);
    virtual void on(const value& val);
};

} // namespace confy

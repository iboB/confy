// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"

namespace confy {

class CONFY_API value : public node {
public:
    // these remain pure virtual from the base class
    // virtual std::string to_string() const noexcept = 0;
    // virtual dict to_dict() const noexcept = 0;

    virtual node* get_child(std::string_view) const noexcept final override {
        return nullptr;
    }
    virtual node* get_abbr_child(std::string_view) const noexcept final override {
        return nullptr;
    }

    virtual void set_from_string(std::string_view str, value_source src) final override;
    virtual void set_from_dict(const dict& d, value_source src) final override;

    virtual void try_set_from_env_var() final override;

    virtual void validate() const final override;

protected:
    virtual void set_value_from_dict(const dict& d) = 0;
    virtual void set_value_from_string(std::string_view str) = 0;
    virtual void validate_value() const = 0;

    bool required = false; // whether this value is required or optional
    value_source m_source = value_source::none; // source from which the value was set
};

} // namespace confy

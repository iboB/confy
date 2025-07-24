// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"
#include <memory>
#include <itlib/flat_map.hpp>

namespace confy {

class CONFY_API section final : public node {
public:
    ~section();

    section(const section&) = delete;
    section& operator=(const section&) = delete;
    section(section&&) noexcept = default;
    section& operator=(section&&) noexcept = default;

    struct dsl : public node::tdsl<section> {
        using node::tdsl<section>::tdsl;
    };

    virtual dict to_dict() const noexcept override;

    virtual node* get_child(std::string_view path) const noexcept override;
    virtual node* get_abbr_child(std::string_view path) const noexcept override;

    virtual void set_from_dict(const dict& d, value_source src) override;

    virtual void validate() const override;

private:
    virtual std::string to_string() const noexcept override;
    virtual void set_from_string(std::string_view str, value_source src) override;

    itlib::flat_map<std::string, std::unique_ptr<node>> m_children;
};

} // namespace confy
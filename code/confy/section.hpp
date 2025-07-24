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
    using node::node;
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

    virtual void try_set_from_env_var() override;
    virtual void validate() const override;

    section& add_section(node_desc desc) {
        auto sec = std::make_unique<section>(std::move(desc), this);
        auto ret = sec.get();
        try_add_child(std::move(sec));
        return *ret;
    }

    template <typename Value>
    typename Value::dsl add_value(node_desc desc) {
        auto val = std::make_unique<Value>(std::move(desc), this);
        auto ret = val.get();
        try_add_child(std::move(val));
        return typename Value::dsl(*ret);
    }

    template <typename T>
    auto add_value(T& ref, node_desc desc) {
        auto val = make_confy_value_from_ref(ref, std::move(desc), this);
        auto ret = val.get();
        try_add_child(std::move(val));
        return typename decltype(val)::element_type::dsl(*ret);
    }

private:
    virtual std::string to_string() const noexcept override;
    virtual void set_from_string(std::string_view str, value_source src) override;

    void try_add_child(std::unique_ptr<node> child);

    itlib::flat_map<std::string, std::unique_ptr<node>> m_children;
};

} // namespace confy
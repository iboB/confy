// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"
#include "ref_value_for.hpp"
#include "node_container.hpp"

namespace confy {

class CONFY_API section : public node {
public:
    using node::node;
    ~section();

    struct dsl : public node::tdsl<section> {
        using node::tdsl<section>::tdsl;
    };

    virtual dict to_dict() const noexcept final override;

    virtual node* get_child(std::string_view path) const noexcept final override;
    virtual node* get_abbr_child(std::string_view path) const noexcept final override;
    virtual std::span<const std::unique_ptr<node>> children() const noexcept final override;

    virtual void set_from_dict(const dict& d, value_source src) final override;

    virtual void try_set_from_env_var() final override;
    virtual void validate() const final override;

    virtual void visit(node_visitor& v) const final override;

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
        auto val = std::make_unique<ref_value_for_t<T>>(ref, std::move(desc), this);
        auto ret = val.get();
        try_add_child(std::move(val));
        return typename decltype(val)::element_type::dsl(*ret);
    }

    struct struct_visitor {
        section& sec;

        template <typename Value>
        typename Value::dsl visit(node_desc desc) {
            return sec.add_value<Value>(std::move(desc));
        }
    };

    template <typename T>
    void add_struct() {}

private:
    virtual std::string to_string() const noexcept final override;
    virtual void set_from_string(std::string_view str, value_source src) final override;

    void try_add_child(std::unique_ptr<node> child);

    node_container m_children;
};

} // namespace confy
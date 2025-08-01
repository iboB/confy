// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node.hpp"

namespace confy {

class node_visitor;

class CONFY_API value : public node {
public:
    using node::node;

    value_source source() const noexcept { return m_source; }
    bool required() const noexcept { return m_required; }

    template <typename Node>
    struct tdsl : public node::tdsl<Node> {
        using NodeDsl = typename Node::dsl;

        using node::tdsl<Node>::tdsl;

        NodeDsl& optional() {
            this->m_node.m_required = false;
            this->m_node.m_source = value_source::none;
            return this->self();
        }

        NodeDsl& required() {
            this->m_node.m_required = true;
            this->m_node.m_source = value_source::none;
            return this->self();
        }
    };

    // these remain pure virtual from the base class
    // virtual std::string to_string() const noexcept = 0;
    // virtual dict to_dict() const noexcept = 0;

    virtual void set_from_string(std::string_view str, value_source src) final override;
    virtual void set_from_dict(const dict& d, value_source src) final override;

    virtual void try_set_from_env_var() final override;

    virtual void validate() const final override;

    virtual void visit(node_visitor& v) const final override;

protected:
    virtual void set_value_from_dict(const dict& d) = 0;
    virtual void set_value_from_string(std::string_view str) = 0;
    virtual void validate_value() const = 0;

    bool m_required = true; // whether this value is required or optional
    value_source m_source = value_source::default_val; // source from which the value was set

private:
    [[noreturn]] void rethrow(const char* ex) const;

    virtual node* get_child(std::string_view) const noexcept final override {
        return nullptr;
    }
    virtual node* get_abbr_child(std::string_view) const noexcept final override {
        return nullptr;
    }
    virtual std::span<const std::unique_ptr<node>> children() const noexcept final override {
        return {};
    }
};

} // namespace confy

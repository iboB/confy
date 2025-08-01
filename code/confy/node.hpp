// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "node_desc.hpp"
#include "dict_fwd.hpp"
#include "value_source.hpp"
#include "env.hpp"
#include <string_view>
#include <span>

namespace confy {

class node_visitor;

class CONFY_API node {
public:
    explicit node(node_desc desc, node* owner) noexcept
        : m_desc(std::move(desc))
        , m_owner(owner)
    {}
    virtual ~node() noexcept;

    node(const node&) = delete;
    node& operator=(const node&) = delete;
    node(node&&) noexcept = delete;
    node& operator=(node&&) noexcept = delete;

    const node* owner() const noexcept { return m_owner; }

    const std::string& name() const noexcept { return m_desc.name; }
    const std::string& desc() const noexcept { return m_desc.description; }
    const std::string& abbr() const noexcept { return m_desc.abbr; }
    const env::var& env_var_data() const noexcept { return m_desc.env_var; }

    std::string get_path() const noexcept;
    std::string get_abbr_path() const noexcept;
    std::string get_env_var_name() const noexcept;

    virtual std::string to_string() const noexcept = 0;
    virtual dict to_dict() const noexcept = 0;

    virtual node* get_child(std::string_view path) const noexcept = 0;
    virtual node* get_abbr_child(std::string_view path) const noexcept = 0;
    virtual std::span<const std::unique_ptr<node>> children() const noexcept = 0;

    virtual void set_from_string(std::string_view str, value_source src) = 0;
    virtual void set_from_dict(const dict& d, value_source src) = 0;

    virtual void try_set_from_env_var() = 0;

    // throw an exception if the node is not valid
    virtual void validate() const = 0;

    virtual void visit(node_visitor& v) const = 0;

    template <typename Node>
    class tdsl {
    public:
        using NodeDsl = typename Node::dsl;

        tdsl(Node& node) : m_node(node) {}

        NodeDsl& name(std::string_view n) {
            m_node.m_desc.name = std::string(n);
            return self();
        }

        NodeDsl& desc(std::string_view d) {
            m_node.m_desc.description = std::string(d);
            return self();
        }

        NodeDsl& abbr(std::string_view a) {
            m_node.m_desc.abbr = std::string(a);
            return self();
        }

        NodeDsl& env_var(std::string_view e) {
            m_node.m_desc.env_var.str = std::string(e);
            m_node.m_desc.env_var.strategy = env::var_strategy::manual;
            return self();
        }

        NodeDsl& env_var_global(std::string_view e) {
            m_node.m_desc.env_var.str = std::string(e);
            m_node.m_desc.env_var.strategy = env::var_strategy::manual_global;
            return self();
        }

        NodeDsl& no_env_var() {
            m_node.m_desc.env_var.strategy = env::var_strategy::none;
            return self();
        }

        Node& val() {
            return m_node;
        }

    protected:
        Node& m_node;
        NodeDsl& self() { return static_cast<NodeDsl&>(*this); }
    };

protected:
    node_desc m_desc;
    const node* m_owner = nullptr;
};

} // namespace confy
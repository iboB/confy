// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string_view>
#include "env.hpp"

namespace confy {

template <typename Node, typename Crtp> // C++23 replace crtp with deducing this
class basic_node_dsl {
public:
    Node& m_node;

    basic_node_dsl(Node& node) : m_node(node) {}

    Crtp& name(std::string_view n) {
        m_node.m_name = std::string(n);
        return self();
    }

    Crtp& desc(std::string_view d) {
        m_node.m_desc = std::string(d);
        return self();
    }

    Crtp& abbr(std::string_view a) {
        m_node.m_abbr = std::string(a);
        return self();
    }

    Crtp& env_var(std::string_view e) {
        m_node.m_env_var.str = std::string(e);
        m_node.m_env_var.strategy = env::var_strategy::manual;
        return self();
    }

    Crtp& env_var_global(std::string_view e) {
        m_node.m_env_var.str = std::string(e);
        m_node.m_env_var.strategy = env::var_strategy::manual_global;
        return self();
    }

    Crtp& no_env_var() {
        m_node.m_env_var.strategy = env::var_strategy::none;
        return self();
    }

    Crtp& required() {
        m_node.required = true;
        return self();
    }

protected:
    Crtp& self() { return static_cast<Crtp&>(*this); }
};

} // namespace confy

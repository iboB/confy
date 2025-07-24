// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "node.hpp"
#include <optional>
#include <cassert>

namespace confy {

node::~node() noexcept = default;

std::string node::get_path() const noexcept {
    if (m_owner) {
        auto n = m_owner->get_path();
        if (!n.empty()) {
            return n + '.' + m_name;
        }
    }
    return m_name;
}

std::string node::get_abbr_path() const noexcept {
    if (m_owner) {
        auto n = m_owner->get_abbr_path();
        if (!n.empty()) {
            return n + '.' + m_abbr;
        }
    }
    return m_abbr;
}

namespace {
std::optional<std::string> construct_env_var_name(const node* n, bool leaf) {
    auto& ev = n->env_var_data();
    if (ev.strategy == env::var_strategy::none) return std::nullopt;

    std::optional<std::string> owner_name;
    if (auto owner = n->owner()) {
        owner_name = construct_env_var_name(owner, false);
        if (!owner_name) return std::nullopt;
    }
    else {
        owner_name.emplace();
    }
    if (ev.strategy == env::var_strategy::manual_global) {
        return ev.str;
    }

    auto& ret = *owner_name;

    if (ev.strategy == env::var_strategy::manual) {
        return ret + ev.str;
    }
    else {
        assert(ev.strategy == env::var_strategy::automatic);

        ret += n->name();
        if (!leaf) {
            ret += '_';
        }
        return ret;
    }
}
}

std::string node::get_env_var_name() const noexcept {
    return construct_env_var_name(this, true).value_or("");
}

} // namespace confy

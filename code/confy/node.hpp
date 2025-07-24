// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "api.h"
#include "dict_fwd.hpp"
#include "value_source.hpp"
#include "env.hpp"
#include <string>
#include <string_view>

namespace confy {

class CONFY_API node {
public:
    virtual ~node() noexcept;

    const node* owner() const noexcept { return m_owner; }
    const std::string& name() const noexcept { return m_name; }
    const std::string& desc() const noexcept { return m_desc; }
    const std::string& abbr() const noexcept { return m_abbr; }

    std::string get_scoped_name() const noexcept {
        if (m_owner) {
            auto n = m_owner->get_scoped_name();
            if (!n.empty()) {
                return n + '.' + m_name;
            }
        }
        return m_name;
    }

    std::string get_scoped_abbr() const noexcept {
        if (m_owner) {
            auto n = m_owner->get_scoped_abbr();
            if (!n.empty()) {
                return n + '.' + m_abbr;
            }
        }
        return m_abbr;
    }

    virtual std::string to_string() const noexcept = 0;
    virtual dict to_dict() const noexcept = 0;

    virtual node* get_child(std::string_view path) const = 0;
    virtual node* get_abbr_child(std::string_view path) const = 0;

    virtual void set_from_string(std::string_view str, value_source src) = 0;
    virtual void set_from_dict(const dict& d, value_source src) = 0;

    // throw an exception if the node is not valid
    // the default implementation does nothing
    virtual void validate() const = 0;

protected:
    friend class configurator;
    template <typename Node, typename Crtp>
    friend class node_dsl;

    const node* m_owner = nullptr;

    std::string m_name;
    std::string m_desc;
    std::string m_abbr;
    env::var m_env_var;
};

} // namespace confy
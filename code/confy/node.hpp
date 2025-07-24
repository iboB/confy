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
    const env::var& env_var_data() const noexcept { return m_env_var; }

    std::string get_path() const noexcept;
    std::string get_abbr_path() const noexcept;
    std::string get_env_var_name() const noexcept;

    virtual std::string to_string() const noexcept = 0;
    virtual dict to_dict() const noexcept = 0;

    virtual node* get_child(std::string_view path) const noexcept = 0;
    virtual node* get_abbr_child(std::string_view path) const noexcept = 0;

    virtual void set_from_string(std::string_view str, value_source src) = 0;
    virtual void set_from_dict(const dict& d, value_source src) = 0;

    virtual void try_set_from_env_var() = 0;

    // throw an exception if the node is not valid
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
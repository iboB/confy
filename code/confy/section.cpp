// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "section.hpp"
#include "dict.hpp"
#include "path_delim.hpp"
#include "node_visitor.hpp"
#include "bits/throw_ex.hpp"

namespace confy {

section::~section() = default;

std::string section::to_string() const noexcept {
    return {};
}
dict section::to_dict() const noexcept {
    auto d = dict::object();
    for (const auto& child : m_children) {
        d[child->name()] = child->to_dict();
    }
    return d;
}

node* section::get_child(std::string_view path) const noexcept {
    if (auto child = m_children.find_by_name(path)) {
        // path points to a child directly
        return child;
    }

    if (auto d = path.find(PATH_DELIM); d != std::string_view::npos) {
        auto child_name = path.substr(0, d);
        if (auto child = m_children.find_by_name(child_name)) {
            return child->get_child(path.substr(d + 1));
        }
        else {
            return nullptr; // no such child
        }
    }

    return nullptr; // no such child
}

std::span<const std::unique_ptr<node>> section::children() const noexcept {
    return m_children;
}

node* section::get_abbr_child(std::string_view path) const noexcept {
    if (auto child = m_children.find_by_abbr(path)) {
        // path points to a child directly
        return child;
    }

    if (auto d = path.find(PATH_DELIM); d != std::string_view::npos) {
        auto child_name = path.substr(0, d);
        if (auto child = m_children.find_by_abbr(child_name)) {
            return child->get_abbr_child(path.substr(d + 1));
        }
        else {
            return nullptr; // no such child
        }
    }

    return nullptr; // no such child
}

void section::set_from_string(std::string_view, value_source) {
    throw_ex{} << get_path() << ": section cannot be set from string";
}

void section::set_from_dict(const dict& d, value_source src) {
    for (const auto& [name, value] : d.items()) {
        if (auto child = m_children.find_by_name(name)) {
            // found a child with the same name
            child->set_from_dict(value, src);
        }
        else {
            // create a new child section or value
            throw_ex{} << get_path() << ": no such child '" << name << "'";
        }
    }
}

void section::try_set_from_env_var() {
    for (const auto& c : m_children) {
        c->try_set_from_env_var();
    }
}

void section::validate() const {
    for (const auto& c : m_children) {
        c->validate();
    }
}

void section::visit(node_visitor& v) const {
    v.on(*this);
}

void section::try_add_child(std::unique_ptr<node> child) {
    auto name = child->name();
    if (m_children.find_by_name(name)) {
        throw_ex{} << get_path() << ": child with name '" << name << "' already exists";
    }
    // search for a child with the same abbreviation
    if (auto abbr = child->abbr(); !abbr.empty()) {
        if (m_children.find_by_abbr(abbr)) {
            throw_ex{} << get_path() << ": child with abbreviation '" << abbr << "' already exists";
        }
    }

    m_children.push_back(std::move(child));
}

} // namespace confy

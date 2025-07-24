// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "section.hpp"
#include "dict.hpp"
#include "path_delim.hpp"
#include "bits/throw_ex.hpp"
#include <itlib/qalgorithm.hpp>

namespace confy {

section::~section() = default;

std::string section::to_string() const noexcept {
    return {};
}
dict section::to_dict() const noexcept {
    dict d;
    for (const auto& [name, child] : m_children) {
        d[name] = child->to_dict();
    }
    return d;
}

node* section::get_child(std::string_view path) const noexcept {
    if (auto it = m_children.find(path); it != m_children.end()) {
        // path point to a child directly
        return it->second.get();
    }

    if (auto d = path.find(PATH_DELIM); d != std::string_view::npos) {
        auto child_name = path.substr(0, d);
        if (auto it = m_children.find(child_name); it != m_children.end()) {
            auto& child = it->second;
            return child->get_child(path.substr(d + 1));
        }
        else {
            return nullptr; // no such child
        }
    }

    return nullptr; // no such child
}

node* section::get_abbr_child(std::string_view path) const noexcept {
    auto p = itlib::pfind_if(m_children.container(), [&](const auto& pair) {
        return pair.second->abbr() == path;
    });
    if (p) {
        return p->second.get();
    }

    if (auto d = path.find(PATH_DELIM); d != std::string_view::npos) {
        auto child_abbr = path.substr(0, d);
        auto child = itlib::pfind_if(m_children.container(), [&](const auto& pair) {
            return pair.second->abbr() == child_abbr;
        });
        if (child) {
            return child->second->get_abbr_child(path.substr(d + 1));
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
        if (auto it = m_children.find(name); it != m_children.end()) {
            // found a child with the same name
            it->second->set_from_dict(value, src);
        }
        else {
            // create a new child section or value
            throw_ex{} << get_path() << ": no such child '" << name << "'";
        }
    }
}

void section::validate() const {
    for (const auto& c : m_children) {
        c.second->validate();
    }
}

} // namespace confy

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "node.hpp"
#include <memory>
#include <vector>

namespace confy {

class node_container : public std::vector<std::unique_ptr<node>> {
public:
    node* find_by_name(std::string_view name) const noexcept {
        for (const auto& n : *this) {
            if (n->name() == name) {
                return n.get();
            }
        }
        return nullptr;
    }

    node* find_by_abbr(std::string_view abbr) const noexcept {
        for (const auto& n : *this) {
            if (n->abbr() == abbr) {
                return n.get();
            }
        }
        return nullptr;
    }
};

} // namespace confy

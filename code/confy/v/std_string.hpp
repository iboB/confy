// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "common_ref_value.hpp"
#include "../dict.hpp"
#include "../bits/throw_ex.hpp"

namespace confy {

class std_string final : public common_ref_value<std::string> {
public:
    using common_ref_value<std::string>::common_ref_value;

    std::string to_string() const noexcept override {
        return m_ref;
    }

    dict to_dict() const noexcept override {
        return {m_ref};
    }

    virtual void set_value_from_string(std::string_view str) override {
        m_ref = std::string(str);
    }

    virtual void set_value_from_dict(const dict& d) override {
        m_ref = d.get<std::string>();
    }
};

inline std::unique_ptr<std_string> make_confy_value_from_ref(std::string& ref, node_desc desc, node* owner) {
    return std::make_unique<std_string>(ref, std::move(desc), owner);
}

}

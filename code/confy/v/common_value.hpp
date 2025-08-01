// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "../value.hpp"
#include "validator.hpp"
#include "common_value_type_traits.hpp"
#include "../ref_value_for.hpp"
#include "../dict.hpp"
#include <memory>
#include <vector>

namespace confy {

template <typename T>
class common_value : public value {
public:
    using traits = common_value_type_traits<T>;
    using ref_type = typename traits::ref_type;
    using const_ref_type = typename traits::const_ref_type;
    using validate_type = typename traits::validate_type;

    common_value(T val, node_desc desc, node* owner) noexcept
        : value(std::move(desc), owner)
        , m_val(val)
    {}

    // this constructor is used for testing
    explicit common_value(T val) noexcept
        : common_value(val, {}, nullptr)
    {}

    common_value(node_desc desc, node* owner) noexcept
        : value(std::move(desc), owner)
        , m_val()
    {}

    common_value() noexcept
        : common_value({}, nullptr)
    {}

    const_ref_type val() const noexcept { return m_val; }

    template <typename U>
    void set_value(U&& v, const value_source src = value_source::manual_override) {
        if (src < m_source) return;
        m_val = std::forward<U>(v);
        m_source = src;
    }

    struct dsl : public value::tdsl<common_value> {
        using value::tdsl<common_value>::tdsl;

        template <typename U>
        dsl& default_val(U&& val) {
            this->m_node.m_val = std::forward<U>(val);
            this->m_node.m_source = value_source::default_val;
            return *this;
        }

        template <template <typename> class Validator, typename... Args>
        dsl& validate(Args&&... args) {
            auto validator = std::make_unique<Validator<validate_type>>(std::forward<Args>(args)...);
            this->m_node.m_validators.push_back(std::move(validator));
            return *this;
        }

        dsl& validate(typename func_validator<validate_type>::func_type func, std::string desc = {}) {
            auto validator = std::make_unique<func_validator<validate_type>>(std::move(func), std::move(desc));
            this->m_node.m_validators.push_back(std::move(validator));
            return *this;
        }
    };

    std::string to_string() const noexcept override {
        return traits::to_string(m_val);
    }

    dict to_dict() const noexcept override {
        return traits::to_dict(m_val);
    }

    virtual void set_value_from_string(std::string_view str) override {
        traits::set_value_from_string(m_val, str);
    }

    virtual void set_value_from_dict(const dict& d) override {
        traits::set_value_from_dict(m_val, d);
    }

    void validate_value() const override {
        for (auto& v : m_validators) {
            if constexpr (traits::deref) {
                v->validate(*m_val);
            }
            else {
                v->validate(m_val);
            }
        }
    }

protected:
    T m_val;
    std::vector<std::unique_ptr<validator<validate_type>>> m_validators;
};

template <typename T>
struct ref_value_for {
    using type = common_value<T&>;
};

} // namespace confy

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "validator.hpp"
#include "../basic_value.hpp"
#include "../basic_value_dsl.hpp"
#include <optional>
#include <memory>
#include <vector>

namespace confy {

template <typename T>
class common_value : public basic_value {
public:
    using value_type = T;

    common_value(T& val) : m_val(val) {}

    struct dsl_type : public basic_value_dsl<common_value, dsl_type> {
        using basic_value_dsl<common_value, dsl_type>::basic_value_dsl;

        dsl_type& default_val(T&& val) {
            this->value.m_default_val = std::forward<T>(val);
            return *this;
        }

        template <template <typename> class Validator, typename... Args>
        dsl_type& validate(Args&&... args) {
            auto validator = std::make_unique<Validator<T>>(std::forward<Args>(args)...);
            this->value.m_validators.push_back(std::move(validator));
            return *this;
        }
    };

    virtual bool try_set_from_default() noexcept override final {
        if (!m_default_val) return false;
        m_val = std::move(*m_default_val);
        m_source = value_source::default_val;
        return true;
    }

    virtual void validate() const override {
        for (auto& v : m_validators) {
            v->validate(m_val);
        }
    }

    virtual std::string get_validation_desc() const noexcept override {
        std::string desc;
        for (const auto& v : m_validators) {
            if (!desc.empty()) desc += "\n";
            desc += v->get_desc();
        }
        return desc;
    }

protected:
    T& m_val;
    std::optional<T> m_default_val;
    std::vector<std::unique_ptr<validator<T>>> m_validators;
};

} // namespace confy

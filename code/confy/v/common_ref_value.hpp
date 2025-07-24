// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "validator.hpp"
#include "../value.hpp"
#include <memory>
#include <vector>

namespace confy {

template <typename T>
class common_ref_value : public value {
public:
    using value_type = T;

    common_ref_value(T& ref) : m_ref(ref) {}

    struct dsl : public value::tdsl<common_ref_value> {
        using value::tdsl<common_ref_value>::tdsl;

        template <typename U>
        dsl& default_val(U&& val) {
            this->m_node.m_ref = std::forward<U>(val);
            this->m_node.m_source = value_source::default_val;
            return *this;
        }

        template <template <typename> class Validator, typename... Args>
        dsl& validate(Args&&... args) {
            auto validator = std::make_unique<Validator<T>>(std::forward<Args>(args)...);
            this->value.m_refidators.push_back(std::move(validator));
            return *this;
        }

        dsl& validate(typename func_validator<T>::func_type func, std::string desc = {}) {
            auto validator = std::make_unique<func_validator<T>>(std::move(func), std::move(desc));
            this->value.m_refidators.push_back(std::move(validator));
            return *this;
        }
    };

protected:
    void validate_value() const override {
        for (auto& v : m_refidators) {
            v->validate(m_ref);
        }
    }

    T& m_ref;
    std::vector<std::unique_ptr<validator<T>>> m_refidators;
};

} // namespace confy

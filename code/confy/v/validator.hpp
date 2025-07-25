// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include <string>
#include <functional>
#include "../exception.hpp"

namespace confy {

template <typename T>
struct validator {
    virtual ~validator() noexcept = default;
    virtual void validate(const T& value) const = 0;
    virtual std::string get_desc() const noexcept = 0;
};

template <typename T>
class func_validator final : public validator<T> {
public:
    using func_type = std::function<bool(const T&)>;

    explicit func_validator(func_type func, std::string desc = {})
        : m_func(std::move(func))
        , m_desc(std::move(desc))
    {}

    void validate(const T& value) const override {
        if (m_func(value)) return;
        throw exception(m_desc);
    }

    std::string get_desc() const noexcept override {
        return m_desc;
    }
private:
    func_type m_func;
    std::string m_desc;
};

} // namespace confy

// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include "exception.hpp"

namespace confy
{

exception::~exception() = default;

schema_exception::schema_exception(std::string text)
    : m_text(std::move(text))
{}
schema_exception::~schema_exception() = default;
const char* schema_exception::what() const noexcept { return m_text.c_str(); }

}

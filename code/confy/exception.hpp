// confy
// Copyright (c) 2020-2021 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "impl/api.h"

#include <exception>
#include <string>
#include <vector>


namespace confy
{

class option;

class CONFY_API exception : public std::exception
{
protected:
    ~exception();
};

class CONFY_API schema_exception : public exception
{
public:
    schema_exception(std::string text);
    ~schema_exception();

    virtual const char* what() const noexcept override;
private:
    std::string m_text;
};

}

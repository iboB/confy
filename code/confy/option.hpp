#pragma once

#include "impl/api.h"

#include "impl/has_key.hpp"

#include "generic_value.hpp"

#include <string>

namespace confy
{

class CONFY_API option : public impl::has_key
{
public:
    virtual ~option();

    const std::string& description() const { return m_description; }
    const std::string& env_var() const { return m_env_var; }

    value_source source() const { return m_source; }

protected:
    virtual bool set_from_default() = 0;
    virtual bool set_from_string(std::string_view str) = 0;

    std::string m_description; // free form description
    std::string m_env_var; // associated environment variable name

    value_source m_source = value_source::none; // source from which the value was set
};

}

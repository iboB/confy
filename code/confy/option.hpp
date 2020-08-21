#pragma once

#include "impl/api.h"

#include "impl/config_item.hpp"

#include "value_source.hpp"

#include <string>

namespace confy
{

class config;

class CONFY_API option : public impl::config_item
{
public:
    virtual ~option();

    const std::string& env_var() const { return m_env_var; }

    value_source source() const { return m_source; }

protected:
    friend class config;
    virtual bool set_from_default() = 0;
    virtual bool set_from_string(std::string_view str) = 0;

    std::string m_description; // free form description
    std::string m_env_var; // associated environment variable name

    value_source m_source = value_source::none; // source from which the value was set
};

}

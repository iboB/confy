#pragma once

#include "api.h"

#include "value_source.hpp"
#include "set_value_result.hpp"

#include <string_view>
#include <iosfwd>

namespace confy
{

class CONFY_API type
{
public:
    virtual void* alloc_value() = 0;
    virtual void dealloc_value(void* ptr) = 0;

    virtual set_value_result set_value(void* target, std::string_view value, value_source source) = 0;

    virtual void write_value(std::ostream& out, const void* value);
    virtual void write_type_name(std::ostream& out);
    virtual void write_type_desc(std::ostream& out);
protected:
};

}

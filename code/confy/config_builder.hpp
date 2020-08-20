#pragma once

#include "impl/api.h"

#include <memory>
#include <string_view>

namespace confy
{

class config;
class section;
class option;

class confing_builder
{
public:
    confing_builder& sec(std::string_view name);

private:
    section* m_cur_section;
    std::unique_ptr<option> m_cur_option;
};

}

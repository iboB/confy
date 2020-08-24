#pragma once

#include "impl/api.h"

#include <memory>
#include <string_view>

namespace confy
{

class config;
class section;
class option;

class CONFY_API schema_dsl
{
public:
    schema_dsl(config& cfg);
    ~schema_dsl() noexcept(false); // this destructor can throw exceptions!

    schema_dsl& sec(std::string_view name, std::string_view abbr = {}, std::string_view desc = {});

    template <typename Option>
    auto opt(std::string_view name, std::string_view abbr = {}, std::string_view desc = {})
    {
        add_cur_option();
        m_cur_option = std::make_unique<Option>(name, abbr, desc);
        return Option::dsl(static_cast<Option&>(*m_cur_option), *this);
    }

private:
    void add_cur_option();
    void add_cur_section();

    config& m_cfg;
    std::unique_ptr<section> m_cur_section;
    std::unique_ptr<option> m_cur_option;
};

}

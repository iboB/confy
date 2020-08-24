#include "schema_dsl.hpp"

#include "option.hpp"
#include "section.hpp"
#include "config.hpp"

namespace confy
{

schema_dsl::schema_dsl(config& cfg)
    : m_cfg(cfg)
    , m_cur_section(std::make_unique<section>())
{
}

schema_dsl::~schema_dsl() noexcept(false)
{
    if (std::uncaught_exceptions()) return; // nothing sensible to do here
    add_cur_option();
    add_cur_section();
}

void schema_dsl::add_cur_option()
{
    if (m_cur_option)
    {
        m_cur_section->add_option(std::move(m_cur_option));
    }
}

void schema_dsl::add_cur_section()
{
    m_cfg.add_section(std::move(m_cur_section));
}


}

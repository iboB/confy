#pragma once

#include "api.h"

#include <string>
#include <string_view>

namespace confy
{
namespace impl
{
class CONFY_API config_item
{
public:
    config_item(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
        : m_name(name)
        , m_abbr(abbr)
        , m_description(desc)
    {}

    const std::string& name() const { return m_name; }
    void set_name(std::string_view n) { m_name = n; }
    const std::string& abbr() const { return m_abbr; }
    void set_abbr(std::string_view n) { m_abbr = n; }
    const std::string& description() const { return m_description; }
    void set_description(std::string_view n) { m_description = n; }

    // template <typename
    // class dsl
    // {
    // public:

    // private:
    //     has_key& m_elem;
    // };
protected:
    std::string m_name;
    std::string m_abbr;
    std::string m_description;
};
}
}

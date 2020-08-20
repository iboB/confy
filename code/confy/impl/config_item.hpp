#pragma once

#include "api.h"

#include <string>

namespace confy
{
namespace impl
{
class CONFY_API config_item
{
public:
    const std::string& name() const { return m_name; }
    const std::string& abbr() const { return m_abbr; }
    const std::string& description() const { return m_description; }

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

#pragma once

#include "api.h"

#include <string>

namespace confy
{
namespace impl
{
class CONFY_API has_key
{
public:
    const std::string& name() const { return m_name; }
    const std::string& abbr() const { return m_abbr; }

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
};
}
}

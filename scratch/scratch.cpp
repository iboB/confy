// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#include <iostream>

#include <confy/confy.hpp>
#include <confy/types/generic_option.hpp>
#include <confy/types/simple_enum.hpp>

using namespace std;

class str : public confy::generic_option<std::string>
{
public:
    str(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {})
        : confy::generic_option<std::string>(name, abbr, desc)
    {
        m_value_type = "string";
        m_value_type_desc = "a text string";
    }

    virtual bool do_set_from_string(std::string& val, std::string_view str)
    {
        val = str;
        return true;
    }
};

namespace confy
{
template <>
struct option_for_type<std::string> { using type = str; };
}

enum xxx
{
    a, b, c
};

int main()
{

    std::string asd = "aAAAa";
    xxx x = c;
    try
    {
        confy::config cfg("foo");

        cfg.setup()
            .cmd_prefix("x:")
            .env_var_prefix("x_")
            .verbose_stream(cout)
            ;

        cfg.schema()
            .opt(asd, "foo", "f", "bar badsa sad as sa")
            .opt(x, "zzz", "b", "enum type")
                .e(a, "a")
                .e(b, "b")
                .e(c, "c")
            .cmd<str>("asd", "z")
                .desc("asdassgd fdzzZZZZZas sa")
                .env("ZXXX")
            ;

        cfg.write_schema(cout);
        //cfg.schema()
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

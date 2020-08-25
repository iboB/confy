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
#include <confy/types/string.hpp>
#include <confy/types/boolean.hpp>

using namespace std;

enum xxx
{
    a, b, c
};

int main()
{

    std::string asd = "aAAAa";
    xxx x = c;
    bool start = false;
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
            .cmd<confy::string>("asd", "z")
                .e("foo")
                .e("bar")
                .e("baz")
                .desc("asdassgd fdzzZZZZZas sa")
                .env("ZXXX")
            .opt(start, "start")
                .desc("start all the stuff when starting app")
            .sec("bbaba", "ba", "the baba stuff")
                .opt(start, "start")
                    .desc("start all the stuff when starting app")
            ;

        cfg.write_schema(cout, true);
        //cfg.schema()
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

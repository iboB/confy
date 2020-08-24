#include <iostream>

#include <confy/confy.hpp>
#include <confy/types/generic_option.hpp>

using namespace std;

namespace confy
{
bool from_string(std::string& out, std::string_view in)
{
    out = in;
    return true;
}
}

class str : public confy::generic_option<std::string>
{
public:
    using confy::generic_option<string>::generic_option;
};

int main()
{

    std::string asd = "aAAAa";
    try
    {
        confy::config cfg("foo");

        cfg.setup()
            .cmd_prefix("x:")
            .env_var_prefix("x_")
            .verbose_stream(cout)
            ;

        cfg.schema()
            .opt<str>("foo", "f")
                .desc("bar badsa sad as sa")
                .val(asd)
            .opt<str>("asd", "z")
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

#include <iostream>

#include <confy/confy.hpp>
#include <confy/types/generic_option.hpp>

using namespace std;

bool from_string(std::string& out, std::string_view in)
{
    out = in;
    return true;
}

class str : public confy::generic_option<std::string>
{
public:
    using confy::generic_option<string>::generic_option;
};

int main() {

    try
    {
        confy::config cfg("foo");

        cfg.setup()
            .cmd_prefix("x:")
            .env_var_prefix("x_")
            .verbose_stream(cout)
            ;

        confy::section sec;

        sec.add_option(std::make_unique<str>("asd", "a", "the asd"));
        sec.add_option(std::make_unique<str>("bbbasd", "b", "the basda asd"));

        cfg.add_section(std::move(sec));

        confy::section sec2("zoo");

        sec2.add_option(std::make_unique<str>("asd", "a", "the asd"));
        sec2.add_option(std::make_unique<str>("bbbasd", "b", "the basda asd"));

        cfg.add_section(std::move(sec2));

        cfg.write_schema(cout);
    }
    catch (std::exception& e)
    {
        cout << e.what() << endl;
    }

    return 0;
}

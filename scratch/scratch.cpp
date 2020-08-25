#include <iostream>

#include <confy/confy.hpp>
#include <confy/types/generic_option.hpp>

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

str* confy_get_option_for_value(std::string&) { return nullptr; }

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
            .opt(asd, "foo", "f", "bar badsa sad as sa")
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

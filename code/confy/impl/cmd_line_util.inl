#include <optional>
#include <string_view>
#include <cstdlib>

namespace
{

constexpr char SECTION_DELIM = '.';

// split argument into section and name
struct path_split
{
    std::string_view section;
    std::string_view option;
};

path_split split_option_path(std::string_view path)
{
    auto pos = path.find_last_of(SECTION_DELIM);
    if (pos == std::string_view::npos) return { {}, path }; // no section

    return { {path.substr(0, pos)}, {path.substr(pos + 1)} };
}

struct parsed_argument
{
    bool relevant = false; // is it relevant to confy
    bool abbr = false; // is it an abbreviated argument
    std::string_view path;
    std::string_view value;
};

constexpr char EQUALS = '=';

// value which is given to unset arguments
constexpr std::string_view UNSET_VALUE = "true";

parsed_argument parse_single_arg(std::string_view arg, std::string_view prefix = {})
{
    if (arg.empty()) return {};
    if (arg[0] != '-') return {};

    parsed_argument ret;

    arg = arg.substr(1);
    if (arg.empty()) return ret;

    if (arg[0] == '-')
    {
        arg = arg.substr(1);
    }
    else
    {
        ret.abbr = true;
    }

    // is it relevant?
    if (arg.compare(0, prefix.length(), prefix) != 0) return ret;

    ret.relevant = true;

    arg = arg.substr(prefix.length());

    auto pos = arg.find(EQUALS);

    if (pos == std::string_view::npos)
    {
        // no equals
        ret.path = arg;
    }
    else
    {
        ret.path = arg.substr(0, pos);
        ret.value = arg.substr(pos + 1);
    }

    return ret;
}

// parse command line confy-style
// func is of the form f(name, is_abbreviated, value)
template <typename F>
void filter_command_line(int& argc, char* argv[], std::string_view prefix, F&& func)
{
    parsed_argument last_p;
    bool wants_value = false;
    int new_argc = 1; // argc after parsing
    for (int i = 1; i < argc; ++i)
    {
        std::string_view arg = argv[i];
        auto p = parse_single_arg(arg, prefix);

        if (wants_value)
        {
            wants_value = false;

            // here we expect a value
            // so we shall interpret an irrelevant arg as a value
            if (!p.relevant)
            {
                func(last_p.path, last_p.abbr, arg);
                continue;
            }
            else
            {
                // p is relevant, this means we'll interpret last arg as true
                func(last_p.path, last_p.abbr, UNSET_VALUE);

                // do not `continue`
                // go on with parsing the relevant arg
            }
        }
        else if (!p.relevant)
        {
            argv[new_argc++] = argv[i];
            continue;
        }

        last_p = p;

        if (p.value.empty())
        {
            wants_value = true;
            continue;
        }

        func(p.path, p.abbr, p.value);
    }

    if (wants_value)
    {
        // we haven't completed the last arg
        func(last_p.path, last_p.abbr, UNSET_VALUE);
    }

    argc = new_argc;
}

}

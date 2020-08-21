#include "config.hpp"

#include "option.hpp"
#include "config_error.hpp"

#include "impl/cmd_line_util.inl"
#include "impl/laurel.inl"

#include <fstream>

namespace confy
{

section::~section() = default;

class config::config_error_manager
{
public:
    bool has_errors() const { return !errors.empty(); }
    void reset() { errors.clear(); }
    std::vector<config_error> errors;
};


config::config(std::string_view name /*= {}*/)
    : m_name(name)
    , m_config_errors(std::make_unique<config_error_manager>())
{}

config::~config() = default;

namespace
{
struct by
{
    bool operator()(const impl::config_item& i) const { return m_cmp(i, *this); }
    bool operator()(const impl::config_item* i) const { return m_cmp(*i, *this); }
    template <typename U>
    bool operator()(const std::unique_ptr<U>& i) const { return m_cmp(*i, *this); }

    static by name(std::string_view n);
    static by abbr(std::string_view a);

    typedef by(*what)(std::string_view val);

    typedef bool(*cmpf)(const impl::config_item& a, const by& b);
    cmpf m_cmp;
    typedef void(*setf)(impl::config_item& target, const by& source);
    setf m_set;
};

template <typename Container>
auto find(Container& c, by what)
{
    return std::find_if(c.begin(), c.end(), what);
}

constexpr std::string_view DEFAULT_VALUE = "default";
constexpr std::string_view CONFIG_FILE = "config";
}

void config::parse_ini_file(std::string_view path)
{
    if (m_open_config_file_func)
    {
        parse_ini_file(*m_open_config_file_func(path));
    }
    else
    {
        std::string spath(path); // convert to something from which we can open an ifstream
        std::ifstream fin(spath);
        parse_ini_file(fin);
    }
}

void config::parse_ini_file(std::istream& in)
{
    section* cur_sec;
    auto on_sec = [&, this](std::string_view sec) {

    };
    auto on_opt = [&, this](std::string_view name, std::string_view value) {

    };
    auto on_error = [&, this](const char* text, int line) {

    };
    laurel(in, on_sec, on_opt, on_error);
}

void config::parse_cmd_line(int& argc, char* argv[])
{
    auto parse_arg = [this](arg_split path, bool is_abbr, std::string_view value) {
        if (path.section.empty() && path.option == CONFIG_FILE)
        {
            // parse config file
            return;
        }

        auto f = is_abbr ? by::abbr : by::name;
        auto sec = find(m_sections, f(path.section));
        if (sec == m_sections.end())
        {
            return;
        }

        auto& options = sec->m_options;
        auto fopt = find(options, f(path.option));

        if (fopt == options.end())
        {
            return;
        }

        auto opt = fopt->get();

        if (value == DEFAULT_VALUE)
        {
            if (!opt->set_from_default())
            {

            }
        }
        else
        {
            if (!opt->set_from_string(value))
            {

            }
        }
    };
}

}

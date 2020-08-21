#pragma once
#include "impl/api.h"

#include <exception>
#include <string>
#include <vector>


namespace confy
{

class option;

class CONFY_API exception : public std::exception
{
protected:
    ~exception();
};

class CONFY_API schema_exception : public exception
{
public:
    schema_exception(std::string text);
    ~schema_exception();

    virtual const char* what() const noexcept override;
private:
    std::string m_text;
};

class CONFY_API config_exception : public exception
{
public:
    struct error
    {
        enum type
        {
            no_suc_section, // configuration referred to a section which was not defined in schema
            no_such_option, // configuration tried to set value to an option that was not defined in schema
            bad_default_value, // default value of option was missing or incompatible
            bad_value, // provided value of option was incompatible
            multiple_value, // value was provided multiple times from the same source
        };

        std::string section_name; // contains the referred section name
        std::string option_name; // contains the referred option name
        bool is_abbr = false; // are the above abbreviated
        std::string provided_value; // what value was provided from the config
        const option* opt = nullptr; // if not null, will contain the option which was referred
    };

    config_exception(std::vector<error> errors);
    ~config_exception();

    const std::vector<error>& errors() const { return m_errors; }

    virtual const char* what() const noexcept override;

private:
    std::vector<error> m_errors;

    // will be filled in what() if it's called
    mutable std::string m_text;
};

}

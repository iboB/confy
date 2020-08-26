// confy
// Copyright (c) 2020 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// https://opensource.org/licenses/MIT
//
#pragma once
#include "../option.hpp"

namespace confy
{
class CONFY_API command : public option
{
public:
    command(std::string_view name = {}, std::string_view abbr = {}, std::string_view desc = {});
    ~command();

    void set_no_arg(bool set = true) { m_no_arg = set; }
    void set_allow_config_file_source(bool allow = true) { m_min_allowed_source = allow ? value_source::config_file : value_source::cmd_line; }

    virtual set_value_result try_set_value(std::string_view val, value_source source) override final;

    virtual set_value_result execute(std::string_view arg) = 0;

    template <typename Option>
    struct dsl_t : public option::dsl_t<Option>
    {
        using option::dsl_t<Option>::dsl_t;

        auto& no_arg(bool set = true)
        {
            this->m_option.set_no_arg(set);
            return this->self();
        }

        auto& allow_config_file_source(bool allow = true)
        {
            this->m_option.set_allow_config_file_source(allow);
            return this->self();
        }
    };
    using dsl = dsl_t<command>;

protected:

    bool m_no_arg = false;
    value_source m_min_allowed_source = value_source::cmd_line;

    std::string m_arg_type;

    virtual bool is_default_false() const final { return m_no_arg; }

    virtual void write_value_type(std::ostream& out) const final;
    virtual void write_value_type_desc(std::ostream& out) const override;
    virtual bool has_default_value() const final { return true; }
    virtual void write_default_value(std::ostream& out) const override;

private:
    // these should never be called
    virtual bool set_from_default() final;
    virtual bool set_from_string(std::string_view str) final;

    // hide
    // otherwise users may cause a crash by adding commands or sections in config::update_options
    using option::m_no_env;
};
}

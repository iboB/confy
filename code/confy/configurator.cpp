// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include "configurator.hpp"
#include "basic_value.hpp"
#include "config.hpp"
#include "bits/value_ex.hpp"
#include <itlib/qalgorithm.hpp>
#include <fstream>
#include <sstream>
#include <cassert>

#include "bits/laurel.inl"

#define VERBOSE(args) \
    if (m_verbose_log_func) { \
        std::ostringstream ss; \
        ss << args; \
        m_verbose_log_func(ss.str()); \
    }

namespace confy {

std::ostream& operator<<(std::ostream& out, const basic_value& bv) {
    auto sec = bv.owner();
    if (sec) {
        out << sec->desc().name << '.';
    }
    out << bv.name();
    return out;
}

configurator::configurator(std::string_view name)
    : m_name(name)
{
    // add default section
    m_sections.push_back(std::make_unique<section>(section::description{}));
}

configurator::~configurator() = default;

void configurator::parse_command_line(int& argc, char* argv[]) {
    VERBOSE(m_name << ": parsing command line");
    for (auto arg : cli::filter_cmd_line(argc, argv, m_cli_prefix)) {
        VERBOSE("  " << arg.key << (arg.abbr ? "(abbr)" : "") << " = " << arg.value);
        m_cli_values.push_back({arg});
    }
}

void configurator::parse_ini_file(std::istream& in, std::string_view filename) {
    VERBOSE(m_name << ": parsing ini config file: " << filename);

    dict* cur_dict = &m_config_file_values;
    auto on_section = [&](std::string_view name) {
        cur_dict = &m_config_file_values[name];
        VERBOSE("  section " << name);
    };
    auto on_key = [&](std::string_view key, std::string_view value) {
        (*cur_dict)[key] = value;
        VERBOSE("     " << key << " = " << value);
    };
    auto on_error = [&](laurel_error error, int line) {
        throw_ex{} << filename << ":" << line << " " << laurel_error_to_text(error);
    };
    laurel(in, on_section, on_key, on_error);
}

void configurator::parse_json_file(std::istream& in, std::string_view filename) {
    try {
        VERBOSE(m_name << ": parsing json config file: " << filename);
        auto json = dict::parse(in);
        VERBOSE(json.dump(2));
        m_config_file_values.merge_patch(json);
    }
    catch (std::exception& ex) {
        throw_ex{} << filename << ": " << ex.what();
    }
}

void configurator::add_manual_overrides(dict overrides) {
    VERBOSE(m_name << ": setting manual overrides: " << overrides);
    m_config_file_values.merge_patch(overrides);
}

configurator::command_result configurator::configure(int& argc, char* argv[]) {
    parse_command_line(argc, argv);
    configure();
    return command_result::continue_exec;
}

namespace {
struct by_name {
    std::string_view name;

    bool operator()(const std::unique_ptr<section>& s) const { return s->desc().name == name; }
    bool operator()(const std::unique_ptr<basic_value>& v) const { return v->name() == name; }
};
}

void configurator::configure() {
    VERBOSE(m_name << ": setting config values");
    _resolve_env_var_names();

    // first go through manual overrides
    // it's an error to have an manual override for non-existent values
    {
        section* cur_section = &get_default_section();
        for (auto& kv : m_manual_overrides.items()) {
            auto* value = itlib::pfind_if(cur_section->m_values, by_name{kv.key()});
            if (!value) {
                if (cur_section != &get_default_section()) {
                    throw_ex{} << "manual override for non-existing value: " << cur_section->desc().name << "." << kv.key();
                }
                else if (auto* sec = find_section(kv.key())) {
                    cur_section = sec;
                }
                else {
                    throw_ex{} << "manual override for non-existing section of value value: " << kv.key();
                }
            }
            else {
                VERBOSE(*value << ": setting manual override from '" << kv.value() << "'");
                (*value)->set_from_dict(kv.value());
                (*value)->m_source = value_source::manual_override;
                (*value)->validate();
                if (auto ca = _find_cli_arg_for_value(**value)) {
                    ca->used = true; // mark as used so as not report it as unused later
                }
            }
        }
    }

    // now go through the sections and search for values in the store
    for (auto& sec : m_sections) {
        dict* sec_cfg_file_dict = nullptr;
        if (sec.get() == &get_default_section()) {
            sec_cfg_file_dict = &m_config_file_values;
        }
        else {
            if (auto f = m_config_file_values.find(sec->desc().name); f != m_config_file_values.end()) {
                sec_cfg_file_dict = &*f;
            }
        }

        for (auto& value : sec->m_values) {
            if (value->m_source == value_source::manual_override) {
                // already set by manual override
                continue;
            }

            // look through the command line arguments
            if (auto ca = _find_cli_arg_for_value(*value)) {
                VERBOSE(*value << ": setting from cli arg: "
                    << ca->key << (ca->abbr ? "(abbr)" : "") << " = " << ca->value);
                value->set_from_string(ca->value);
                value->m_source = value_source::cmd_line;
                value->validate();
                ca->used = true; // mark as used
                continue;
            }

            // look in the config file values
            if (sec_cfg_file_dict) {
                auto it = sec_cfg_file_dict->find(value->name());
                if (it != sec_cfg_file_dict->end()) {
                    VERBOSE(*value << ": setting from config file store value: " << *it);
                    if (it->type() != dict::value_t::string) {
                        value->set_from_string(it->get<std::string_view>());
                    }
                    else {
                        value->set_from_dict(*it);
                    }
                    value->m_source = value_source::config_file;
                    value->validate();
                    continue;
                }
            }

            // look in the environment variables
            if (!value->m_env_var.str.empty()) {
                auto env_value = std::getenv(value->m_env_var.str.c_str());
                if (env_value) {
                    VERBOSE(*value << ": setting from environment variable: "
                        << value->m_env_var.str << " = " << env_value);
                    value->set_from_string(env_value);
                    value->m_source = value_source::env_var;
                    value->validate();
                    continue;
                }
                else {
                    VERBOSE("Env var " << value->m_env_var.str << " for " << *value << " is not set")
                }
            }

            // try default value
            if (value->try_set_from_default()) {
                VERBOSE(*value << ": setting from default value");
                value->m_source = value_source::default_val;
                value->validate();
                continue;
            }


            // unset value, check if it's required
            if (value->required) {
                value_ex{*value} << "required value not set";
            }


            VERBOSE(*value << ": not set");
        }
    }

    // check for unused command line arguments
    for (auto& ca : m_cli_values) {
        if (!ca.used) {
            throw_ex{} << "Command line argument not used: "
                << ca.key << (ca.abbr ? " (abbr)" : "") << " = " << ca.value;
        }
    }

    m_configure_done = true;
}

section& configurator::add_section(section::description desc) {
    auto sec = std::make_unique<section>(std::move(desc));
    auto& ret = *sec;
    m_sections.push_back(std::move(sec));
    return ret;
}
section& configurator::get_default_section() const noexcept {
    return *m_sections.front();
}

section* configurator::find_section(std::string_view name) const noexcept {
    auto it = itlib::pfind_if(m_sections, by_name{name});
    if (!it) return nullptr;
    return it->get();
}

configurator::cli_arg* configurator::_find_cli_arg_for_value(basic_value& val) {
    auto* sec = val.owner();
    assert(sec);

    for (auto& arg : m_cli_values) {
        std::string_view name = arg.key;

        auto [qsec, qval] = [&] {
            if (arg.abbr) {
                return std::make_pair(sec->desc().abbr, name);
            }
            else {
                return std::make_pair(sec->desc().name, name);
            }
        }();

        if (!name.starts_with(qsec)) continue;
        name.remove_prefix(qsec.size());
        if (name.starts_with(cli::KEY_DELIM)) {
            name.remove_prefix(1);
        }
        if (name == val.name()) {
            return &arg;
        }
    }
    return nullptr;
}

config configurator::get_config() {
    if (!m_configure_done) {
        throw_ex{} << "Configuration is not done yet. Call configure() first.";
    }
    config cfg;
    cfg.m_name = m_name;
    cfg.m_sections = std::move(m_sections);
    return cfg;
}

void configurator::_resolve_env_var_names() {
    for (auto& sec : m_sections) {
        for (auto& value : sec->m_values) {
            if (m_no_env
                || sec->desc().env_var.strategy != env::var_strategy::none
                || value->m_env_var.strategy != env::var_strategy::none
            ) {
                // env vars are blocked
                value->m_env_var.str.clear();
                continue;
            }

            if (value->m_env_var.strategy == env::var_strategy::manual_global) {
                // keep original
                continue;
            }

            std::string env_var_name;
            if (sec->desc().env_var.strategy == env::var_strategy::manual_global) {
                env_var_name = sec->desc().env_var.str;
            }
            else {
                env_var_name = m_env_var_prefix;
                if (sec->desc().env_var.strategy != env::var_strategy::manual) {
                    env_var_name += sec->desc().env_var.str;
                }
                else {
                    assert(sec->desc().env_var.strategy == env::var_strategy::automatic);
                    env_var_name += sec->desc().name + '_';
                }
            }

            if (value->m_env_var.strategy != env::var_strategy::manual) {
                env_var_name += value->m_env_var.str;
            }
            else {
                assert(value->m_env_var.strategy == env::var_strategy::automatic);
                env_var_name += value->name();
            }

            value->m_env_var.str = std::move(env_var_name);
        }
    }
}

} // namespace confy

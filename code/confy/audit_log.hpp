// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#pragma once
#include "node_visitor.hpp"
#include "value.hpp"
#include "section.hpp"
#include <ostream>

namespace confy {

class audit_log_visitor final : public node_visitor {
    std::ostream& m_out;
public:
    audit_log_visitor(std::ostream& out) noexcept
        : m_out(out)
    {}

    void on(const section& sec) override {
        for (auto& c : sec.children()) {
            c->visit(*this);
        }
    }
    void on(const value& val) override {
        m_out << val.get_path();
        if (val.source() == value_source::none) {
            m_out << " <UNSET>\n";
            return;
        }
        m_out << "=" << val.to_string() << "\n";
        m_out << "  (source: ";
        switch (val.source()) {
            case value_source::none: m_out << "???"; break; // here for the warning's sake
            case value_source::default_val: m_out << "default value"; break;
            case value_source::env_var: m_out << "environment variable"; break;
            case value_source::config_file: m_out << "config file"; break;
            case value_source::cmd_line: m_out << "command line"; break;
            case value_source::manual_override: m_out << "manual user override"; break;
        }
        m_out << ")\n";
    }
};

inline void audit_log(const section& sec, std::ostream& out) {
    audit_log_visitor v{out};
    sec.visit(v);
}

} // namespace confy

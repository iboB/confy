// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <cstdlib>

#if defined(WIN32)
// ::SetEnvironmentVariable doesn't work with std::getenv :(

#include <vector>
#include <string_view>

namespace {
std::vector<std::vector<char>> vars;
void setenv(std::string_view name, std::string_view value, int) {
    std::vector<char> new_var(name.length() + value.length() + 1 /*=*/ + 1 /*\0*/);
    char* str = new_var.data();
    memcpy(str, name.data(), name.length());
    str += name.length();
    *str++ = '=';
    memcpy(str, value.data(), value.length());
    str += value.length();
    *str = 0;

    auto& v = vars.emplace_back(std::move(new_var));
    _putenv(v.data());
}

void unsetenv(std::string_view name) {
    setenv(name, "", 1);
}
} // namespace

#endif

namespace {
void set_env_var(const char* name, const char* value) {
    if (value) {
        setenv(name, value, 1);
    }
    else {
        unsetenv(name);
    }
}
} // namespace


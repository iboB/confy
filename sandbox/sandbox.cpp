// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/config.hpp>

#include <iostream>
#include <cstdint>
#include <string>

using namespace std;

struct my_config {
    uint16_t port;
    std::string host;
    std::string resource;
    bool allow = false;

    template <typename Visitor>
    void confy_visit(Visitor& v) {
        v("port", port, "The port to connect to");
        v("host", host, "The host to connect to");
        v("resource", resource, "The resource to access");
        v("allow", allow, "Whether to allow the operation");
    }
};

int main() {
    return 0;
}

// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <confy/config.hpp>
#include <confy/v/integer.hpp>
#include <confy/v/std_string.hpp>

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
    void confy_build_config(Visitor& v) {
        v(port, "port", "p", "The port to connect to");
        v(host, "host", "H", "The host to connect to");
        v(resource, "resource", "r", "The resource to access");
        v(allow, "allow", "a", "Allow access to the resource", false);
    }
};

int main() try {
    uint16_t port = 0;
    confy::integer<uint16_t> cport(port);
    cport.set_from_string("xxs", confy::value_source::manual_override);
    return 0;
}
catch (std::exception& e) {
    cerr << "Exception: " << e.what() << endl;
    return 1;
}
catch (...) {
    cerr << "Unknown exception" << endl;
    return 1;
}

# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
include(icm_add_lib)

icm_add_lib(confy CONFY)
add_library(confy::confy ALIAS confy)

CPMAddPackage(gh:iboB/splat@1.3.3)
CPMAddPackage(gh:iboB/itlib@1.11.7)
CPMAddPackage(gh:nlohmann/json@3.12.0)

target_link_libraries(confy PUBLIC
    splat::splat
    itlib::itlib
    nlohmann_json::nlohmann_json
)

target_sources(confy
    INTERFACE FILE_SET HEADERS FILES
        confy/api.h
        confy/bits/cli_util.hpp

        confy/exception.hpp

        confy/config.hpp

        confy/node.hpp
        confy/section.hpp
        confy/value.hpp
        confy/node_visitor.hpp


        confy/configurator.hpp
    PRIVATE
        confy/bits/cli_util.cpp

        confy/exception.cpp

        confy/config.cpp

        confy/node.cpp
        confy/section.cpp
        confy/value.cpp
        confy/node_visitor.cpp

        confy/configurator.cpp
)

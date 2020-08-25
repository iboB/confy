# confy
# Copyright (c) 2020 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
set(src
    confy/impl/api.h
    confy/impl/config_item.hpp

    confy/config.hpp
    confy/config.cpp
    confy/exception.hpp
    confy/exception.cpp
    confy/option.hpp
    confy/option.cpp
    confy/schema_dsl.hpp
    confy/schema_dsl.cpp
    confy/value_source.hpp

    confy/confy.hpp
)

if(CONFY_STATIC_LIB)
    add_library(confy STATIC ${src})
else()
    add_library(confy SHARED ${src})

    if(UNIX)
        target_compile_options(confy PRIVATE -fvisibility=hidden)
    endif()

    target_compile_definitions(confy
        PUBLIC -DCONFY_SHARED=1
    )
endif()

target_compile_definitions(confy
    PRIVATE -DCONFY_IMPLEMENT=1
)

target_include_directories(confy PUBLIC .)

# option(CONFY_GENERATE_SINGLE_HEADER "Confy-dev: generate single header" ${dev_mode})

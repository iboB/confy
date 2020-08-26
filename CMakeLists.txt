# confy
# Copyright (c) 2020 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.10)

project(confy)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # dev_mode is used below to make life simpler for developers
    # it enables some configurations and the defaults for building tests and examples
    # which typically wouldn't be built if the library is a subdirectory of another project
    set(dev_mode ON)
else()
    set(dev_mode OFF)
endif()

option(CONFY_BUILD_TESTS "Confy: build tests" ${dev_mode})
option(CONFY_BUILD_EXAMPLES "Confy: build examples" ${dev_mode})
option(CONFY_BUILD_SCRATCH "Confy: build scratch project for testing and experiments" ${dev_mode})

mark_as_advanced(CONFY_BUILD_TESTS CONFY_BUILD_EXAMPLES CONFY_BUILD_SCRATCH)

option(CONFY_SINGLE_HEADER "Confy: use a single header file" OFF)
option(CONFY_STATIC_LIB "Confy: build as a static library (if off, build as a shared lib)" OFF)

if(dev_mode)
    include(./dev.cmake)
endif()

if(CONFY_SINGLE_HEADER)
    add_library(confy INTERFACE)
    target_include_directories(confy INTERFACE single-header)
else()
    add_subdirectory(code)
endif()

if(CONFY_BUILD_SCRATCH)
    add_subdirectory(scratch)
endif()

if(CONFY_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(CONFY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

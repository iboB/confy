# confy
# Copyright (c) 2020-2021 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(./get_cpm.cmake)

CPMAddPackage(
    NAME icm
    GITHUB_REPOSITORY iboB/icm
    VERSION 1.1.1
)

CPMAddPackage(
    NAME doctest-lib
    GITHUB_REPOSITORY iboB/doctest-lib
    GIT_TAG 2.4.3b
)

add_library(confy-impl INTERFACE)
target_include_directories(confy-impl INTERFACE ../code/confy)

include(${icm_SOURCE_DIR}/icm_testing.cmake)

macro(add_impl_test test)
    icm_add_doctest_lib_test(${test} confy-impl ${ARGN})
endmacro()

add_impl_test(cmd_line_util t-cmd_line_util.cpp)
add_impl_test(laurel t-laurel.cpp)
add_impl_test(setenv t-setenv.cpp)
add_impl_test(num_from_string t-num_from_string.cpp)

macro(add_confy_test test)
    icm_add_doctest_lib_test(${test} confy ${ARGN})
endmacro()

add_confy_test(types
    t-types.cpp
    t-simple_enum.cpp
    t-auto_enum.cpp
)

add_confy_test(confy
    t-confy.cpp
)

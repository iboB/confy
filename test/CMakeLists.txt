# confy
# Copyright (c) 2020 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
add_library(doctest STATIC
    doctest.hpp
    doctest_main.cpp
)

macro(add_lib_test test lib)
    set(tname test-${lib}-${test})
    add_executable(${tname} ${ARGN})
    target_link_libraries(${tname} doctest ${lib})
    set_target_properties(${tname} PROPERTIES FOLDER test)
    add_test(NAME ${test} COMMAND $<TARGET_FILE:${tname}>)
endmacro()

add_library(confy-impl INTERFACE)
target_include_directories(confy-impl INTERFACE ../code/confy)

macro(add_impl_test test)
    add_lib_test(${test} confy-impl ${ARGN})
endmacro()

add_impl_test(cmd_line_util t-cmd_line_util.cpp)
add_impl_test(laurel t-laurel.cpp)
add_impl_test(setenv t-setenv.cpp)
add_impl_test(num_from_string t-num_from_string.cpp)

macro(add_confu_test test)
    add_lib_test(${test} confy ${ARGN})
endmacro()

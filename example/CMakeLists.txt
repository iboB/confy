# confy
# Copyright (c) 2020-2021 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
macro(add_example name)
    set(ename confy-example-${name})
    add_executable(${ename} ${ARGN})
    target_link_libraries(${ename} confy)
    set_target_properties(${ename} PROPERTIES FOLDER example)
endmacro()

add_example(basic e-basic.cpp)

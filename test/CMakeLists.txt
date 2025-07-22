# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/doctest-util@0.1.3)
set_target_properties(doctest PROPERTIES FOLDER test)
set_target_properties(doctest-main PROPERTIES FOLDER test)

macro(confy_test test)
    add_doctest_lib_test(${test} confy t-${test}.cpp ${ARGN})
endmacro()

confy_test(setenv)
confy_test(cli)
